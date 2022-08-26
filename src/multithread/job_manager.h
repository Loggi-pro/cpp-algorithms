#pragma once
#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include <iostream>
#include <optional>
#include <string>

#if __cplusplus >= 202002L
#include <concepts>
#ifndef IS_CPP20_SUPPORT
#define IS_CPP20_SUPPORT
#endif
#endif

class IJob {
protected:
	enum class Status {
		Queued,
		InProcess,
		Cancelled,
		Finished,
	};
	std::vector<std::shared_ptr<IJob>> _thenJobs;
	std::atomic<int> _countToWait = 0;
	std::atomic<Status> status = Status::Queued;
	std::atomic_bool cancelled_after_start;
	std::string debugName{};

	void done() {
		for (auto& j : _thenJobs) { j->_countToWait--; }

		_thenJobs.clear();
	}

public:
	IJob() = default;
	IJob(std::string name = {}) : debugName(std::move(name)) {}
	IJob(const IJob& other) = delete;
	virtual ~IJob() = default;
	virtual void run() = 0;

	std::string_view getDebugName() const {
		return debugName;
	}

	bool isCancelled() const {
		return status == Status::Cancelled || cancelled_after_start;
	}

	bool isReadyToRun() const {
		return _countToWait == 0;
	}
	void then(std::shared_ptr<IJob> j) {
		j->_countToWait++;
		_thenJobs.push_back(std::move(j));
	}


	void cancel() {
		if (status == Status::InProcess || status == Status::Queued) {
			cancelled_after_start = true;

			for (auto& j : _thenJobs) {
				j->_countToWait--;
				j->cancel();// cancel all sequenced jobs
			}

			_thenJobs.clear();
		}
	}
#ifdef IS_CPP20_SUPPORT
	void join() {
		status.wait(Status::InProcess);// c++20 feature;
		assert(status == Status::Finished || status == Status::Cancelled);
	}
#endif
	std::string& getName() {
		return debugName;
	}
};

#ifdef IS_CPP20_SUPPORT
template<typename T>
concept JobCallable = std::invocable<T, const IJob&>;
#endif

template<class Callable>
class CustomJob : public IJob {
	Callable _func;

public:
#ifdef IS_CPP20_SUPPORT
	explicit CustomJob(JobCallable auto func, std::string name = {}) : IJob(std::move(name)), _func(func) {}
#else
	explicit CustomJob(Callable func, std::string name = {}) : IJob(std::move(name)), _func(func) {}
#endif

	void run() override {
		if (cancelled_after_start) {
			status = Status::Cancelled;
		} else {
			status = Status::InProcess;
			_func(*this);
			done();
			status = Status::Finished;
		}
#ifdef IS_CPP20_SUPPORT
		status.notify();
#endif
	}
};

struct TaskHandle {
	std::shared_ptr<IJob> job;
	void cancel() {
		job->cancel();
	}
};


enum class BLOCKING_POLICY : int {
	NO_BLOCK,
	BLOCK,
};


namespace details {

class notification_queue {
	std::deque<std::shared_ptr<IJob>> _q;
	bool _done{ false };
	std::mutex _mutex;
	std::condition_variable _ready;

	bool extract(std::shared_ptr<IJob>& x) {
		for (auto& f : _q) {
			if (f->isReadyToRun()) {
				if (f != _q.front()) { f.swap(_q.front()); }

				x = std::move(_q.front());
				_q.pop_front();
				return true;
			}
		}

		return false;
	}

public:
	notification_queue() = default;

	bool try_pop(std::shared_ptr<IJob>& x) {
		std::unique_lock<std::mutex> lock{ _mutex, std::try_to_lock };

		if (!lock || _q.empty()) { return false; }

		return extract(x);
	}

	bool pop(BLOCKING_POLICY policy, std::shared_ptr<IJob>& x) {
		std::unique_lock<std::mutex> lock{ _mutex };

		if (_q.empty() && !_done && policy == BLOCKING_POLICY::BLOCK) { _ready.wait(lock); }

		if (_q.empty()) { return false; }

		return extract(x);
	}

	bool try_push(std::shared_ptr<IJob>& f) {
		{
			std::unique_lock<std::mutex> lock{ _mutex, std::try_to_lock };

			if (!lock) { return false; }

			_q.emplace_back(std::move(f));
		}
		_ready.notify_one();
		return true;
	}

	void push(std::shared_ptr<IJob> f) {
		{
			std::unique_lock<std::mutex> lock{ _mutex };
			_q.emplace_back(std::move(f));
		}
		_ready.notify_one();
	}

	bool empty() const {
		return _q.empty();
	}

	void terminate() {
		{
			std::unique_lock<std::mutex> lock{ _mutex };
			_done = true;
		}
		_ready.notify_all();
	}
};

}// namespace details

// extract job from queues
class JobExtractor {
	static constexpr std::size_t K_DEFAULT = 2;
	const std::size_t K;// count to repeat
	std::atomic<unsigned int> _index{ 0 };
	std::vector<details::notification_queue> _queue;

public:
	explicit JobExtractor(std::size_t size)
		: K(size == 1 ? 1 : K_DEFAULT), _queue(size)// not overlap for size==1
	{}
	~JobExtractor() {
		_queue.clear();
	}

	void push(std::shared_ptr<IJob> job) {
		auto i = _index++;
		const std::size_t len = _queue.size() * K;

		for (std::size_t n = 0; n != len; ++n) {
			std::size_t index = (i + n) % _queue.size();

			if (_queue[index].try_push(job)) { return; }
		}

		_queue[i % _queue.size()].push(std::move(job));
	}

	std::shared_ptr<IJob> pop(BLOCKING_POLICY policy, std::size_t id) {
		std::shared_ptr<IJob> result{};
		const std::size_t len = _queue.size();

		for (std::size_t n = 0; n != len; ++n) {
			const std::size_t index = (id + n) % len;

			if (_queue[index].try_pop(result)) { break; }
		}

		if (!result) {
			_queue[id].pop(policy, result);// wait job here or cancel
		}

		return result;
	}

	bool empty() const {
		for (auto& q : _queue) {
			if (!q.empty()) { return false; }
		}

		return true;
	}

	void terminate_all() {
		for (auto& q : _queue) { q.terminate(); }
	}
};

// thread abstraction
class IThreadHandler {
public:
	static inline constexpr std::size_t UI_THREAD_IDX = 0;
	static inline std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();
	IThreadHandler() = default;
	virtual void doWork() = 0;
	virtual ~IThreadHandler() {}
	virtual bool isAttachedToThread() const = 0;
};

class JobManager;
class MultiThreadHandler : public IThreadHandler {
	std::vector<std::thread> _threads;

public:
	template<typename Callabale>
	MultiThreadHandler(JobManager& jobManager, Callabale f, std::size_t count);

	void doWork() final {}// make sense only for ui thread

	~MultiThreadHandler() override {
		join();
	}


	void join() {
		for (auto& t : _threads) { t.join(); }
	}

	bool isAttachedToThread() const final {
		for (auto& t : _threads) {
			if (t.get_id() == std::this_thread::get_id()) { return true; }
		}

		return false;
	}
};

class UiThreadHandler : public IThreadHandler {
public:
	std::function<void(std::size_t)> func;
	template<typename Callabale>
	UiThreadHandler(JobManager& jobManager, Callabale f);
	~UiThreadHandler() override;
	void doWork() final {// make sense only for ui thread
		func(IThreadHandler::UI_THREAD_IDX);// UI_THREAD_IDX
	}

	bool isAttachedToThread() const final {
		return IThreadHandler::MAIN_THREAD_ID == std::this_thread::get_id();
	}
};

// Job manager base class
class JobManager {
protected:
	std::atomic<unsigned int> _countInWork = 0;
	JobExtractor queue;
	std::atomic_bool isTermination = false;
	std::unique_ptr<IThreadHandler> _handler;

	void run_internal(BLOCKING_POLICY policy, std::size_t idx) {
		std::shared_ptr<IJob> f = queue.pop(policy, idx);
		if (!f) { return; }
		f->run();
		_countInWork--;
	}

	JobManager(std::unique_ptr<IThreadHandler> handler, std::size_t threadCount)
		: queue(threadCount), _handler(std::move(handler)) {
		// do after all
	}

public:
	template<typename F>
	std::optional<TaskHandle> post(F f, std::string name = {}) {
		if (isTermination) { return std::nullopt; }
		_countInWork++;
		std::shared_ptr<CustomJob<F>> job = std::make_shared<CustomJob<F>>(std::move(f), std::move(name));
		queue.push(job);// push copy
		return TaskHandle{ std::move(job) };
	}

	std::optional<TaskHandle> post(std::shared_ptr<IJob> f) {
		if (isTermination) { return std::nullopt; }
		_countInWork++;
		queue.push(f);// push copy
		return TaskHandle{ std::move(f) };
	}


	virtual ~JobManager() {
		terminate();
		waitTillCompletion();
		queue.terminate_all();
	}

	void terminate() {
		isTermination = true;
	}

	virtual void waitTillCompletion(){};
};

// Job manager for multithreading
class MtJobManager : public JobManager {
private:
	std::atomic_bool isInited = false;

	std::unique_ptr<IThreadHandler> createHandler(std::size_t threadCount) {
		auto l = [this](std::size_t idx) mutable {
			while (!isInited) { continue; }// wait till not inited
			while (!isTermination) {
				// run in loop with blocking policy
				this->run_internal(BLOCKING_POLICY::BLOCK, idx);
			}
		};
		return std::make_unique<MultiThreadHandler>(*this, l, threadCount);
	}

public:
	// it autostort
	explicit MtJobManager(std::size_t threadCount = std::thread::hardware_concurrency())
		: JobManager(createHandler(threadCount), threadCount) {
		isInited = true;
	}

	void waitTillCompletion() final {
		while (_countInWork > 0) { continue; }
	}
};

// Job manager for Ui thread
class UiJobManager : public JobManager {
	std::unique_ptr<IThreadHandler> createHandler() {
		auto l = [this](std::size_t idx) mutable {
			// run once with no_block policy
			this->run_internal(BLOCKING_POLICY::NO_BLOCK, idx);
		};
		return std::make_unique<UiThreadHandler>(*this, l);
	}

public:
	// not autostart: need manual calling loop!
	UiJobManager() : JobManager(createHandler(), 1) {}

	void loop() {
		assert(_handler->isAttachedToThread());
		// no block thread => run only once
		_handler->doWork();
	}

	void waitTillCompletion() override final {
		while (_countInWork > 0) { loop(); }
	}
};

class CurrentThreadJobManager {
	thread_local static inline JobManager* _jobManager = nullptr;

public:
	static inline JobManager* get() {
		return _jobManager;
	}
	static void set(JobManager* jobManager) {
		_jobManager = jobManager;
	}
};

template<typename Callabale>
inline MultiThreadHandler::MultiThreadHandler(JobManager& jobManager, Callabale f, std::size_t count)
	: IThreadHandler() {
	_threads.reserve(count);

	for (unsigned int n = 0; n != count; ++n) {
		_threads.emplace_back([n = n, func = f, ptrJobMng = &jobManager]() mutable {
			CurrentThreadJobManager::set(ptrJobMng);
			func(n);
			CurrentThreadJobManager::set(nullptr);
		});
	}
}

template<typename Callabale>
inline UiThreadHandler::UiThreadHandler(JobManager& jobManager, Callabale f) : IThreadHandler(), func(f) {
	// UI job manager can only be created from main thread;
	assert(std::this_thread::get_id() == IThreadHandler::MAIN_THREAD_ID);
	CurrentThreadJobManager::set(&jobManager);
}

inline UiThreadHandler::~UiThreadHandler() {
	CurrentThreadJobManager::set(nullptr);
}