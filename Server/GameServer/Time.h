#pragma once
#include <chrono>

using TimePoint = std::chrono::steady_clock::time_point;

class Time
{
public:
	Time(uint32 updateRate = UPDATE_RATE);
	~Time();

	void Init();

	static auto GetNowTime() { return std::chrono::high_resolution_clock::now(); }
	static bool IsTimeOver(TimePoint& time);
	static TimePoint GetRemainingTime(uint32 second);

	bool IsAbleUpdate();
	
private:
	const std::chrono::milliseconds _updateInterval;

	TimePoint _lastUpdateTime;
};
