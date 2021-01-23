#pragma once
//

struct NotificationHandle {
private:
	using notify_ptr = void (*)(void *, void *);
	friend struct IObservable;
	void *object;
	notify_ptr func;
	NotificationHandle *next = nullptr;

public:
	template<typename T>
	void call(T *arg) {
		func(object, (void *)arg);
	}
	NotificationHandle *getLast() {
		NotificationHandle *handle = this;
		while (handle->next != nullptr) {
			handle = handle->next;// get next element in list
		}
		return handle;
	}
	NotificationHandle(void *context, notify_ptr f) : object(context), func(f) {}
	template<auto f, typename T>
	static NotificationHandle make(T *obj) {
		notify_ptr caller{ [](void *ctx, void *data) { (static_cast<T *>(ctx)->*f)(data); } };
		return { obj, caller };
	}
};

struct IObservable {
private:
	NotificationHandle *_handle{ nullptr };
	int _nSubscribers{ 0 };

public:
	void subscribe(NotificationHandle *newHandle) {
		if (_handle == nullptr) {
			_handle = newHandle;
		} else {
			_handle->getLast()->next = newHandle;
		}
		// pNH now points to an constructed notificationHandle;
		++_nSubscribers;
	}
	bool unsubscribe(NotificationHandle *handle) {
		NotificationHandle *pNH = _handle;
		NotificationHandle *pBack = _handle;
		if (pNH == nullptr) {// empty list
			return false;
		}
		// first one?
		if (pNH == handle) {
			_handle = pNH->next;
			--_nSubscribers;
			return true;
		}
		// search list to find
		while (pNH != nullptr) {
			if (pNH == handle) {
				pBack->next = pNH->next;
				--_nSubscribers;
				return true;
			}// end if found
			pBack = pNH;// points to the list element before pNH
			pNH = pNH->next;// get next element in list
		}
		return false;
	}
	void notify(void *data) const {
		NotificationHandle *pNH = _handle;
		while (pNH != nullptr) {
			pNH->call(data);
			pNH = pNH->next;
		}
	}
};
