#pragma once
#include <chrono>
#include <limits>
#include <windows.h>

#ifdef max
#undef max
#endif

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

template<typename Dst, typename Src>
inline Dst numeric_cast(Src value) {
	using DstLim = std::numeric_limits<Dst>;
	using SrcLim = std::numeric_limits<Src>;

	static constexpr bool positive_overflow_possible = DstLim::max() < SrcLim::max();
	static constexpr bool negative_overflow_possible = SrcLim::is_signed or (DstLim::lowest() > SrcLim::lowest());

	// unsigned <-- unsigned
	if constexpr ((not DstLim::is_signed) and (not SrcLim::is_signed)) {
		if (positive_overflow_possible and (value > DstLim::max())) {
			throw std::overflow_error(__PRETTY_FUNCTION__ + std::string(": positive overflow"));
		}
	}
	// unsigned <-- signed
	else if constexpr ((not DstLim::is_signed) and SrcLim::is_signed) {
		if (positive_overflow_possible and (value > DstLim::max())) {
			throw std::overflow_error(__PRETTY_FUNCTION__ + std::string(": positive overflow"));
		} else if (negative_overflow_possible and (value < 0)) {
			throw std::overflow_error(__PRETTY_FUNCTION__ + std::string(": negative overflow"));
		}

	}
	// signed <-- unsigned
	else if constexpr (DstLim::is_signed and (not SrcLim::is_signed)) {
		if (positive_overflow_possible and (value > DstLim::max())) {
			throw std::overflow_error(__PRETTY_FUNCTION__ + std::string(": positive overflow"));
		}
	}
	// signed <-- signed
	else if constexpr (DstLim::is_signed and SrcLim::is_signed) {
		if (positive_overflow_possible and (value > DstLim::max())) {
			throw std::overflow_error(__PRETTY_FUNCTION__ + std::string(": positive overflow"));
		} else if (negative_overflow_possible and (value < DstLim::lowest())) {
			throw std::overflow_error(__PRETTY_FUNCTION__ + std::string(": negative overflow"));
		}
	}

	// limits have been checked, therefore safe to cast
	return static_cast<Dst>(value);
}

class Event {
	struct EventHandle_;
	using EventHandle = EventHandle_*;

public:
	static inline const std::chrono::milliseconds WAIT_INFINITE{ DWORD(-1) };

	Event(bool isManualReset = false, bool isInitialSignaled = false)
		: ev(static_cast<EventHandle>(
			CreateEvent(nullptr, static_cast<BOOL>(isManualReset), static_cast<BOOL>(isInitialSignaled), nullptr))) {}

	Event(const Event& other) = delete;
	void operator=(const Event& other) = delete;
	Event(Event&& other) noexcept : ev(other.ev) {
		other.ev = nullptr;
	}

	virtual ~Event() {
		CloseHandle(ev);
	}

	void set() {
		SetEvent(ev);
	}
	void reset() {
		ResetEvent(ev);
	}

	bool wait(std::chrono::milliseconds msTimeout = WAIT_INFINITE) {
		assert(msTimeout.count() <= WAIT_INFINITE.count());
		auto ms = numeric_cast<DWORD>(msTimeout.count());
		const auto res = static_cast<WaitResult>(WaitForSingleObject(ev, ms));
		return res == WaitResult::SIGNAL0;
	}


private:
	EventHandle ev;
	enum class WaitResult : DWORD { SIGNAL0 = WAIT_OBJECT_0, TIMEOUT = WAIT_TIMEOUT, ERR = WAIT_FAILED };
};