#include <gtest/gtest.h>

#include "algorithms/fibonacci.h"
#include "multithread/event.h"
#include "multithread/future.h"
#include "multithread/job_manager.h"

#include "../test_helpers.h"
#include "job_manager_test_helpers.h"

static void testWithJobManager(JobManager&& mng) {
	std::vector<std::string> jobSequence;
	std::mutex m;
	auto lwork = [&](const IJob& j) {
		fibbonacci_matrix<std::size_t>(1'000u);
		std::lock_guard lock(m);
		jobSequence.emplace_back(j.getDebugName());
	};
	std::shared_ptr<IJob> job1 = std::make_shared<CustomJob<decltype(lwork)>>(lwork, "job1");
	auto lsync = [&](const IJob& j) {
		std::lock_guard lock(m);
		jobSequence.emplace_back(j.getDebugName());
		std::this_thread::sleep_for(std::chrono::milliseconds(1000u));
	};
	std::shared_ptr<IJob> job2 = std::make_shared<CustomJob<decltype(lsync)>>(lsync, "job2");
	job1->then(job2);
	mng.post(job2);
	mng.post(job1);
	mng.waitTillCompletion();
	EXPECT_EQ(2, jobSequence.size());// all jobs are did work
	EXPECT_EQ("job1", jobSequence[0]);
	EXPECT_EQ("job2", jobSequence[1]);
}

TEST(JobManagerTest, CalcFibonacciFromMainThread) {
	// auto start = clock();
	testWithJobManager(UiJobManager{});
	// double time_taken = double(clock() - start) / double(CLOCKS_PER_SEC);
	// std::cout << "Time taken by program is : " << std::fixed << time_taken << std::setprecision(5) << " sec " << std::endl;
}

TEST(JobManagerTest, CalcFibonacciFromOtherThread) {
	// auto start = clock();
	testWithJobManager(MtJobManager{ 2 });
	// double time_taken = double(clock() - start) / double(CLOCKS_PER_SEC);
	// std::cout << "Time taken by program is : " << std::fixed << time_taken << std::setprecision(5) << " sec " << std::endl;
}

static MtJobManager task_manager{};
static UiJobManager ui_manager{};

static std::vector<TestEvent> events;

struct JobWidget : public StatusWidget {
	void update_func() final {
		TaskHandle task = task_manager
												.post(
													[this](const IJob& token) mutable {
														int status = getStatusInBackgroundTask(std::chrono::milliseconds(100u));
														events.push_back(TestEvent::RUN_OTHER_THREAD_ACTION);
														if (doBeforeFirstCancel) { doBeforeFirstCancel(); }
														if (token.isCancelled()) { return; }

														ui_manager.post(
															[this, status = std::move(status)](const IJob& token) mutable {
																if (doBeforeSecondCancel) { doBeforeSecondCancel(); }
																if (token.isCancelled() || progress->isCancelled) { return; }
																events.push_back(TestEvent::RUN_UI_CALLBACK);
																progress->stop();
																// UPDATE_UI HERE
																(void)status;
															},
															"after get status");
													},
													"start get status")
												.value();
		progress->setOnCancel([task = std::move(task)]() mutable {
			task.cancel();
			events.push_back(TestEvent::CANCEL_TASK);
		});
	}
};

TEST(JobManagerTest, ComplexWidgetTest_ShouldBeOk) {
	events.clear();
	JobWidget widget;
	widget.update();
	task_manager.waitTillCompletion();
	ui_manager.loop();
	EXPECT_ARR_EQ(SINGLE_ARG({ TestEvent::RUN_OTHER_THREAD_ACTION, TestEvent::RUN_UI_CALLBACK }), events);
}

TEST(JobManagerTest, ComplexWidgetTest_OnCancelOnExternalTask) {
	events.clear();
	JobWidget widget;
	widget.doBeforeFirstCancel = [&]() {
		ui_manager.post([&](const IJob& /*unused*/) { widget.progress->cancel(); });
		ui_manager.loop();
	};

	widget.update();
	task_manager.waitTillCompletion();
	EXPECT_EQ(true, ui_manager.empty());
	EXPECT_ARR_EQ(SINGLE_ARG({ TestEvent::RUN_OTHER_THREAD_ACTION, TestEvent::CANCEL_TASK }), events);
}

TEST(JobManagerTest, ComplexWidgetTest_OnCancelInInnerTask) {
	events.clear();
	JobWidget widget;
	widget.doBeforeSecondCancel = [&]() { widget.progress->cancel(); };

	widget.update();
	task_manager.waitTillCompletion();
	ui_manager.loop();
	EXPECT_EQ(true, ui_manager.empty());
	EXPECT_ARR_EQ(SINGLE_ARG({ TestEvent::RUN_OTHER_THREAD_ACTION, TestEvent::CANCEL_TASK }), events);
}
