#pragma once
#include "observer.h"
#include <iostream>
#include <optional>
#include <vector>
// common data
struct TimeMarkedData {
	int time;
};

// queue with time data
struct Queue : public IObservable {
private:
	int _head{ 0 };
	int _size{ 0 };
	std::vector<TimeMarkedData> _buffer;

public:
	void insert(TimeMarkedData tmd) {
		_buffer.push_back(tmd);
		notify(&tmd);
	}
	[[nodiscard]] bool isEmpty() const {
		return _buffer.empty();
	}
	std::optional<TimeMarkedData> get(std::size_t index) {
		if (index < _buffer.size()) { return {}; }
		return { _buffer.at(index) };
	}
};

// data receiver
struct HistogrammDisplay {
	NotificationHandle handle;
	void (*on_display)(TimeMarkedData *data);
	explicit HistogrammDisplay(Queue *q) : handle(NotificationHandle::make<&HistogrammDisplay::display>(this)) {
		// subscribe here
		q->subscribe(&handle);
	}
	void display(void *d) {
		TimeMarkedData *data = static_cast<TimeMarkedData *>(d);
		if (on_display != nullptr) { on_display(data); }
	}
};