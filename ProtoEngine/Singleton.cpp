#include "Singleton.h"

// Phase 1 Singletons -- begin
#include "ConfigMgr.h"
#include "Profiler.h"
// Phase 1 Singletons -- end

// Phase 2 Singletons -- begin 
#include "EffectMgr.h"
// Phase 2 Singletons -- end 

#include "RenderInterface.h"

void initOrClearPhaseOneSingletons( bool isInit);
void initOrClearPhaseTwoSingletons( bool isInit, RenderInterface* ri);

void initPhaseOneSingletons()
{
	initOrClearPhaseOneSingletons(true);
}

void clearPhaseOneSingletons()
{
	initOrClearPhaseOneSingletons(false);
}

void initPhaseTwoSingletons(RenderInterface* ri)
{
    initOrClearPhaseTwoSingletons(true, ri);
}

void clearPhaseTwoSingletons(RenderInterface* ri)
{
    initOrClearPhaseTwoSingletons(false, ri);
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

void initOrClearPhaseTwoSingletons( bool isInit, RenderInterface* ri)
{
    guard(EffectMgr)
        if (isInit)
            Singleton<EffectMgr>::getInstance().init(ri);
        else
            Singleton<EffectMgr>::getInstance().destroy();
    unguard

    guard(ShaderMgr)
        if (isInit)
            Singleton<ShaderMgr>::getInstance().init(ri);
        else
            Singleton<ShaderMgr>::getInstance().destroy();
    unguard

}


