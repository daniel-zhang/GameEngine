#include "Singleton.h"

// Phase 1 Singletons -- begin
#include "ConfigMgr.h"
#include "Profiler.h"
// Phase 1 Singletons -- end

// Phase 2 Singletons -- begin 
// Phase 2 Singletons -- end 


void initOrClearPhaseOneSingletons( bool isInit);
void initOrClearPhaseTwoSingletons( bool isInit);

void initPhaseOneSingletons()
{
	initOrClearPhaseOneSingletons(true);
}

void clearPhaseOneSingletons()
{
	initOrClearPhaseOneSingletons(false);
}

void initPhaseTwoSingletons()
{
    initOrClearPhaseTwoSingletons(true);
}

void clearPhaseTwoSingletons()
{
    initOrClearPhaseTwoSingletons(false);
}

#define guard(x) {
#define unguard }

void initOrClearPhaseOneSingletons( bool isInit)
{
    guard(ConfigMgr)	
		if (isInit)
            Singleton<ConfigMgr>::getInstance().init();
		else
            Singleton<ConfigMgr>::getInstance().destroy();
    unguard
	 
	guard(Profiler) 
		if (isInit)
			Singleton<Profiler>::getInstance().init();
		else
			Singleton<Profiler>::getInstance().destroy();
	unguard 
}

void initOrClearPhaseTwoSingletons( bool isInit )
{
    guard()
        if (isInit)
            ;
        else
            ;
    unguard
}


