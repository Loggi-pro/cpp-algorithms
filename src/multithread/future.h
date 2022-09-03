#pragma once
#include "job_manager.h"

#if __cplusplus >= 202002L
#include <concepts>
#ifndef IS_CPP20_SUPPORT
#define IS_CPP20_SUPPORT
#endif
#endif

// std::invokable<Value,const CancellationToken& token>
#ifndef IS_CPP20_SUPPORT
template<typename Callable>
struct Future {
	friend Future<Callable> async(Callable func, JobManager& jobManager);
	using Value = std::invoke_result_t<Callable, const IJob&>;
	Callable _func;
	JobManager& jobManager;
	std::optional<TaskHandle> task;

public:
	template<typename Callable2>
	void then(Callable2 callback, JobManager& jobManagerForCallback) {
		task = jobManager.post(
			[this, func = std::move(_func), callback = std::move(callback), &jobManagerForCallback](const IJob& job) mutable {
				Value value = func(job);

				if (job.isCancelled()) { return; }

				if (&jobManagerForCallback == &jobManager) {
					// run from same thread
					callback(std::move(value), job);
				} else {
					// run in other thread
					jobManagerForCallback.post([callback = std::move(callback), value = std::move(value)](const IJob& job) mutable {
						if (job.isCancelled()) { return; }

						callback(std::move(value), job);
					});
				}
			});
	}
	void cancel() {
		assert(task);
		task->cancel();
	}
};

template<typename Callable>
auto async(Callable func, JobManager& jobManager) {
	return Future<Callable>{ std::move(func), jobManager, std::nullopt };
}
#else
template<typename T>
concept AsyncFunction = std::invocable<T, const IJob&>;

template<typename T, typename Value>
concept ThenFunction = std::invocable<T, Value, const IJob&>;

template<AsyncFunction Callable>
struct Future {
	// friend Future<Callable> async(Callable func, JobManager& jobManager);
	using Value = std::invoke_result_t<Callable, const IJob&>;
	Callable _func;
	JobManager& jobManager;
	std::optional<TaskHandle> task;

public:
	void then(ThenFunction<Value> auto callback, JobManager& jobManagerForCallback) {
		task = jobManager.post(
			[this, func = std::move(_func), callback = std::move(callback), &jobManagerForCallback](const IJob& job) {
				Value value = func(job);

				if (job.isCancelled()) { return; }

				if (&jobManagerForCallback == &jobManager) {
					// run from same thread
					callback(std::move(value), job);
				} else {
					// run in other thread
					jobManagerForCallback.post([callback = std::move(callback), value = std::move(value)](const IJob& job) {
						if (job.isCancelled()) { return; }

						callback(std::move(value), job);
					});
				}
			});
	}
	void cancel() {
		assert(task);
		task->cancel();
	}
};


auto async(AsyncFunction auto func, JobManager& jobManager) {
	return Future<decltype(func)>{ std::move(func), jobManager, std::nullopt };
}
#endif
