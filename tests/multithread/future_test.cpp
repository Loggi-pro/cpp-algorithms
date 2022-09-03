#include <gtest/gtest.h>

#include "multithread/event.h"
#include "multithread/future.h"
#include "multithread/job_manager.h"

#include "../test_helpers.h"
#include "job_manager_test_helpers.h"


static MtJobManager task_manager{};
static UiJobManager ui_manager{};

static std::vector<TestEvent> events;

struct FutureWidget : public StatusWidget {
	void update_func() final {
		auto future = async(
			[this](const IJob& /*unused*/) mutable -> int {
				const int status = getStatusInBackgroundTask(std::chrono::milliseconds(100u));
				events.push_back(TestEvent::RUN_OTHER_THREAD_ACTION);
				if (doBeforeFirstCancel) { doBeforeFirstCancel(); }
				return status;
			},
			task_manager);
		future.then(
			[this](int status, const IJob& job) mutable {
				if (doBeforeSecondCancel) { doBeforeSecondCancel(); }
				if (progress->isCancelled) { return; }
				events.push_back(TestEvent::RUN_UI_CALLBACK);
				progress->stop();
				// UPDATE_UI HERE
			},
			ui_manager);
		progress->setOnCancel([future = std::move(future)]() mutable {
			future.cancel();
			events.push_back(TestEvent::CANCEL_TASK);
		});
	}
};

TEST(FutureTest, ComplexWidgetTest_ShouldBeOk) {
	events.clear();
	FutureWidget widget;
	widget.update();
	task_manager.waitTillCompletion();
	ui_manager.loop();
	EXPECT_ARR_EQ(SINGLE_ARG({ TestEvent::RUN_OTHER_THREAD_ACTION, TestEvent::RUN_UI_CALLBACK }), events);
}

TEST(FutureTest, ComplexWidgetTest_OnCancelOnExternalTask) {
	events.clear();
	FutureWidget widget;
	widget.doBeforeFirstCancel = [&]() {
		ui_manager.post([&](const IJob& /*unused*/) { widget.progress->cancel(); });
		ui_manager.loop();
	};

	widget.update();
	task_manager.waitTillCompletion();
	EXPECT_EQ(true, ui_manager.empty());
	EXPECT_ARR_EQ(SINGLE_ARG({ TestEvent::RUN_OTHER_THREAD_ACTION, TestEvent::CANCEL_TASK }), events);
}

TEST(FutureTest, ComplexWidgetTest_OnCancelInInnerTask) {
	events.clear();
	FutureWidget widget;
	widget.doBeforeSecondCancel = [&]() { widget.progress->cancel(); };

	widget.update();
	task_manager.waitTillCompletion();
	ui_manager.loop();
	EXPECT_EQ(true, ui_manager.empty());
	EXPECT_ARR_EQ(SINGLE_ARG({ TestEvent::RUN_OTHER_THREAD_ACTION, TestEvent::CANCEL_TASK }), events);
}

// TODO Add coro Future

/*
template<typename T>
concept CoroAsyncFunction = std::invocable<T, const IJob&>;

template<typename T, typename Value>
concept CoroThenFunction = std::invocable<T, Value, const IJob&>;

template <AsyncFunction Callable>
struct CoroFuture : std::suspend_always {
	//friend Future<Callable> async(Callable func, JobManager& jobManager);
	using Value = std::invoke_result_t<Callable, const IJob&>;
	Callable _func;
	Value result = {};
	std::shared_ptr<IJob> job;
	JobManager& _jobManager;

	auto make_job(AsyncFunction auto f, JobManager& jobManager) {
		return std::make_shared<CustomJob>([this, createdContext = CurrentThreadJobManager::get(), f = std::move(f)]
																				 (const IJob & job) {
					 result = f(job);

					 if (!job.isCancelled()) {
						 //createdContext.post
					 }
		});
	}

public:
	CoroFuture(AsyncFunction auto f,
						 JobManager& jobManager): job(make_job(jobManager)), _jobManager(jobManager) {
	}

	Value await_resume() { return result; }

	void await_suspend(std::coroutine_handle<>) {
		_jobManager.post(job);
		job->join();
	}
	void cancel() {
		//assert(task);
		//task->cancel();
	}
};


struct CoroWidget {

	void update_func() final {
		//progress->setOnCancel([future = std::move(future)]() mutable { future.cancel(); }); must be done by set owner, owner
must have interface to cancel SomePromiseJob& job = createPromise(task_manager); // must save current jobManager to resume after
		auto future = job.async([](const IJob&)->int { return getStatus(); }, task_manager);
		int status = co_await future;
		//continue in saved jobManager(ui thread)
		progress->stop();
		update_ui(status);
	}
};
*/