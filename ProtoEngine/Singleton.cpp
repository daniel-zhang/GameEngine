
// Singletons -- begin
#include "Configuration.h"
#include "Profiler.h"
// Singletons -- end
#include "Singleton.h"

void singletonCreateDestroy( bool isInit);

void initAllSingletons()
{
	singletonCreateDestroy(true);
}

void destroyAllSingletons()
{
	singletonCreateDestroy(false);
}

#define guard(x) {
#define unguard }

void singletonCreateDestroy( bool isInit)
{
    guard(Configuration)	
		if (isInit)
		{
            Singleton<Configuration>::getInstance().init();
		}
		else
		{
            Singleton<Configuration>::getInstance().destroy();
		}
    unguard
	 
	guard(Profiler) 
		if (isInit)
		{
			Singleton<Profiler>::getInstance().init();
		}
		else
		{
			Singleton<Profiler>::getInstance().destroy();
		}
	unguard 
}

