#include "Ticker.h"
#include <windows.h>

Ticker::Ticker() : 
mSecPerCount(0.0), 
	mDeltaTime(-1.0),
	mBaseTime(0),
	mPausedTime(0),
	mPrevTime(0),
	mCurrTime(0),
	mStopped(false)
{
	__int64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	mSecPerCount = 1.0 / (double)frequency;
}

float Ticker::totalTime() const
{
	if(mStopped)
	{
		return (float)(((mStopTime - mPausedTime)-mBaseTime)*mSecPerCount);
	}
	else
	{
		return (float)(((mCurrTime-mPausedTime)-mBaseTime)*mSecPerCount);
	}
}

float Ticker::delta_second() const
{
	return (float)mDeltaTime;
}

void Ticker::start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	// If timer is paused
	if(mStopped)
	{
		mPausedTime += (startTime - mStopTime);
		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void Ticker::stop()
{
	if(!mStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mStopTime = currTime;
		mStopped = true;
	}
}

void Ticker::reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void Ticker::tick()
{
	if(mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;
	mDeltaTime = (mCurrTime - mPrevTime) * mSecPerCount;

	mPrevTime = mCurrTime;

	// Force non-negative delta time. Delta time can be negative in CPU power save mode, or when the program is switched to another CPU core.
	if(mDeltaTime < 0.0)
		mDeltaTime = 0.0;
}

float Ticker::delta_millisecond() const
{
    return 1000.f * (float)mDeltaTime;
}
