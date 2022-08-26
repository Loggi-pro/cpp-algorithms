#include "patterns/observer_example.h"
#include <gtest/gtest.h>

static int called_times = 0;
TEST(ObserverTest, ShouldCallNotifyOnDisplay_OnEveryIsertToQueue) {

	Queue q;
	HistogrammDisplay display{ &q };

	called_times = 0;
	display.on_display = [](TimeMarkedData *tmd) {
		called_times++;
		EXPECT_EQ(0, tmd->time);
	};
	// call first time
	q.insert(TimeMarkedData{ 0 });
	display.on_display = [](TimeMarkedData *tmd) {
		called_times++;
		EXPECT_EQ(1, tmd->time);
	};
	// call second time
	q.insert(TimeMarkedData{ 1 });
	display.on_display = [](TimeMarkedData *tmd) {
		called_times++;
		EXPECT_EQ(2, tmd->time);
	};
	// call third time
	q.insert(TimeMarkedData{ 2 });
	EXPECT_EQ(3, called_times);
}
