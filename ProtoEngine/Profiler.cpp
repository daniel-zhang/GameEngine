#include "Profiler.h"
#include "Windows.h"

Timer::Timer( std::wstring name, std::wstring desc )
{
	mName = name;
	mDesc = desc;
	mParent = NULL;
	mEnabled = true;

	reset();
}

Timer::~Timer()
{
}

void Timer::setParent( Timer* parent)
{
	mParent = parent;
}

void Timer::addChild( Timer* child )
{
	child->setParent(this);
	mChildren.push_back(child);
}

void Timer::reset()
{
	mTotalCalls = 0;
	mTotalTicks = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&mFrequency);

	mCurrInterval.reset();
	mMaxInterval.reset();
	mMinInterval.reset();
	mMinInterval.delta = uint64(-1);
}

uint32 Timer::totalCalls()
{
	return mTotalCalls;
}

float Timer::totalTime()
{
	return (float)(mTotalTicks * 1.0 / mFrequency);
}

void Timer::start()
{
	mCurrInterval.reset();
	QueryPerformanceCounter((LARGE_INTEGER*)&(mCurrInterval.start));

	mTotalCalls++; 
}

void Timer::end()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&(mCurrInterval.delta));
	mCurrInterval.delta = mCurrInterval.delta - mCurrInterval.start;

	mTotalTicks += mCurrInterval.delta;

	if (mCurrInterval.delta < mMinInterval.delta)
	{
		mMinInterval.start = mCurrInterval.start;
		mMinInterval.delta = mCurrInterval.delta;
	}
	else if (mCurrInterval.delta > mMaxInterval.delta)
	{
		mMaxInterval.start = mCurrInterval.start;
		mMaxInterval.delta = mCurrInterval.delta;
	}
}


Profiler::Profiler()
{
	mRootTimer = NULL;
}

Profiler::~Profiler()
{
	clearAll();
}

bool Profiler::init()
{
	mRootTimer = new Timer(L"root", L"The root timer");
	mTimers.push_back(mRootTimer);

	return true;
}

bool Profiler::destroy()
{
	clearAll();
	return true;
}

void Profiler::clearAll()
{
	for (uint32 i = 0; i < mTimers.size(); ++i)
	{
		if (mTimers[i])
		{
			delete mTimers[i];
			mTimers[i] = NULL;
		}
	}
	mTimers.clear();
}

Timer* Profiler::createTimer( std::wstring name, std::wstring desc, std::wstring parentName, bool enabled )
{
	Timer* timer = NULL;
	// Random access only happens when creating timer 
	// Finding timer parent by string comparison is acceptable
	for (uint32 i = 0; i < mTimers.size(); ++i)
	{
		if (parentName.compare(mTimers[i]->mName) == 0)
		{
			Timer* parent = mTimers[i];

			timer = new Timer(name, desc);
			timer->mEnabled = enabled;
			parent->addChild(timer);

			mTimers.push_back(timer);
		}
	}

	return timer;
}

void Profiler::enableTimer( Timer* timer )
{
	timer->mEnabled = true;
}

void Profiler::disableTimer( Timer* timer )
{
	timer->mEnabled = false;
}

void Profiler::enableAll()
{
	for (uint32 i = 0; i < mTimers.size(); ++i )
		mTimers[i]->mEnabled = true;
}

void Profiler::disableAll()
{
	for (uint32 i = 0; i < mTimers.size(); ++i )
		mTimers[i]->mEnabled = false;
}

void Profiler::startTimer( Timer* timer )
{
	timer->start();
}

void Profiler::endTimer( Timer* timer )
{
	timer->end();
}


