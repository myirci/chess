#pragma once

#include <chrono>

class BenchmarkTimer
{
public:

	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
	using TimeDuration = std::chrono::duration<double>;

	enum class TimeUnit : uint8_t
	{
		nanoseconds,
		microseconds,
		milliseconds,
		seconds,
		minutes,
		hours,
		days
	};

	BenchmarkTimer(TimeUnit unit) : _unit{ unit } { }

	void Start() 
	{
		_start = std::chrono::steady_clock::now();
	}

	void End() 
	{
		_end = std::chrono::steady_clock::now();
	}

	TimeUnit GetUnit() const { return _unit; }

	void SetUnit(TimeUnit unit) { _unit = unit; }

	double GetDuration() const 
	{
		TimeDuration duration = _end - _start;
		
		switch (_unit) 
		{
		case TimeUnit::nanoseconds:
			return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
		case TimeUnit::microseconds:
			return (double)std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
		case TimeUnit::milliseconds:
			return (double)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		case TimeUnit::seconds:
			return (double)std::chrono::duration_cast<std::chrono::seconds>(duration).count();
		case TimeUnit::minutes:
			return (double)std::chrono::duration_cast<std::chrono::minutes>(duration).count();
		case TimeUnit::hours:
			return (double)std::chrono::duration_cast<std::chrono::hours>(duration).count();
		case TimeUnit::days:
			return (double)std::chrono::duration_cast<std::chrono::days>(duration).count();
		default:
			return -1.0;
		}
	}

	std::string GetUnitStr() const 
	{
		switch (_unit)
		{
		case TimeUnit::nanoseconds:
			return "nanoseconds";
		case TimeUnit::microseconds:
			return "microseconds";
		case TimeUnit::milliseconds:
			return "milliseconds";
		case TimeUnit::seconds:
			return "seconds";
		case TimeUnit::minutes:
			return "minutes";
		case TimeUnit::hours:
			return "hours";
		case TimeUnit::days:
			return "days";
		default:
			return "Error";
		}
	}
private:
	TimePoint _start, _end;
	TimeUnit _unit;
};

