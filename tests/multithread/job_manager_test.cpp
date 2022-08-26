#include <gtest/gtest.h>

#include "algorithms/fibonacci.h"
#include "multithread/job_manager.h"

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
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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

TEST(JobManagerTest, CalcFibonacciFromMultiThread) {
	// auto start = clock();
	testWithJobManager(MtJobManager{ 2 });
	// double time_taken = double(clock() - start) / double(CLOCKS_PER_SEC);
	// std::cout << "Time taken by program is : " << std::fixed << time_taken << std::setprecision(5) << " sec " << std::endl;
}