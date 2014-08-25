#ifndef TICKER_H
#define TICKER_H

#include "typedefs.h"

class Ticker
{
public:
	Ticker();

	float totalTime() const;		// in seconds
	float delta_second() const;		// in seconds
	float delta_millisecond() const;		// in ms

	void start();
	void stop();
	void reset();
	void tick();

private:
	double mSecPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};

#endif
