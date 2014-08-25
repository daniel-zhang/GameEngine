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
    float lastDelta_second();
    float lastDelta_millisecond();

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

class Profiler : public Singleton<Profiler>
{
	// Make singleton -- start
protected:
	Profiler();
	~Profiler();
public:
	friend class Singleton<Profiler>;
	bool init();
	bool destroy();
	// Make singleton -- end 

public:
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