#pragma once
#include <chrono>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

struct ProgressBar {
	bool isStarted = false;
	bool isCancelled = false;
	std::function<void()> _onCancel;
	std::shared_ptr<std::thread> t;
	std::mutex _m;

	void stop() {
		std::unique_lock<std::mutex> lock{ _m };

		if (isStarted) {
			isStarted = false;
			// std::cout << "stop progress bar\n";
			t->join();
			t.reset();
		}
	}
	template<typename T>
	void setOnCancel(T func) {
		_onCancel = std::function<void()>([this, f = func]() mutable {
			isCancelled = true;
			f();
		});
	}

	void start() {
		isStarted = true;
		isCancelled = false;
		t = std::make_shared<std::thread>([this]() {
			while (isStarted) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
				// std::cout << ".";
			}
		});
	}
	void cancel() {
		if (isStarted) {
			stop();

			if (_onCancel) { _onCancel(); }
		}
	}
	~ProgressBar() {
		if (isStarted) { cancel(); }
	}
};

struct StatusWidget {
	int a = 0;
	std::shared_ptr<ProgressBar> progress;
	std::function<void()> doBeforeFirstCancel = nullptr;
	std::function<void()> doBeforeSecondCancel = nullptr;

	void update() {
		progress = std::make_shared<ProgressBar>();
		update_func();
		progress->start();
	}

	virtual void update_func() = 0;
	virtual ~StatusWidget() = default;
};

inline int getStatusInBackgroundTask(std::chrono::milliseconds waitFor) {
	std::this_thread::sleep_for(waitFor);
	return 10;
}

enum class TestEvent : int { RUN_OTHER_THREAD_ACTION, RUN_UI_CALLBACK, CANCEL_TASK };

template<>
struct std::formatter<TestEvent> : std::formatter<std::string> {
	auto format(TestEvent ev, format_context& ctx) {
		const char* str = [&]() {
			switch (ev) {
			case TestEvent::RUN_OTHER_THREAD_ACTION:
				return "RUN_OTHER_THREAD_ACTION";
			case TestEvent::RUN_UI_CALLBACK:
				return "RUN_UI_CALLBACK";
			case TestEvent::CANCEL_TASK:
				return "CANCEL_TASK";
			}
			return "UNKNOWN";
		}();
		return formatter<string>::format(std::format("{}", str), ctx);
	}
};
