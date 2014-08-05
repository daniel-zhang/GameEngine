#ifndef PROFILER_H
#define PROFILER_H

#include "reference.h"
#include <string>
#include <vector>
#include "Singleton.h"

class Timer
{
public:
	uint32 totalCalls();
	float totalTime();

protected:
	friend class Profiler;
	Timer(std::wstring name, std::wstring desc);
	~Timer();

	void setParent(Timer* parent);
	void addChild(Timer* child);
	void reset();
	void start();
	void end();

	struct Interval
	{
		void reset(){start = 0; delta = 0;}
		uint64 start;
		uint64 delta;
	};

protected:
	bool mEnabled;
	std::wstring mName;
	std::wstring mDesc;

	uint32 mTotalCalls;
	uint64 mTotalTicks;
	uint64 mFrequency;

	Interval mCurrInterval;
	Interval mMaxInterval;
	Interval mMinInterval;

	Timer* mParent;
	std::vector<Timer*> mChildren;
};

class Profiler 
{
	// Make singleton
	friend class Singleton<Profiler>;
protected:
	Profiler();
	~Profiler();

public:
	bool init();
	bool destroy();

	Timer* createTimer(std::wstring name, std::wstring desc, std::wstring parentName = L"root", bool enabled = true);
	void enableTimer(Timer* timer);
	void disableTimer(Timer* timer);
	void enableAll();
	void disableAll();
	void clearAll();

	void startTimer(Timer* timer);
	void endTimer(Timer* timer);


protected:
	Timer* mRootTimer;
	std::vector<Timer*> mTimers;
};

#endif