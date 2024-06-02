#include "pch.h"
#include "Time.h"

Time::Time(uint32 updateRate /*= UPDATE_RATE*/)
	: _updateInterval(std::chrono::milliseconds(1000) / updateRate)
{
	_lastUpdateTime = GetNowTime();
}

Time::~Time()
{
	
}

void Time::Init()
{
	_lastUpdateTime = GetNowTime();
}

bool Time::IsTimeOver(TimePoint& time)
{
	return time <= Time::GetNowTime() ? true : false;
}

TimePoint Time::GetRemainingTime(uint32 second)
{
	return Time::GetNowTime() + std::chrono::seconds(second);
}

bool Time::IsAbleUpdate()
{
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(GetNowTime() - _lastUpdateTime);

	if (deltaTime < _updateInterval)
		return false;

	_lastUpdateTime += deltaTime;

	return true;
}
