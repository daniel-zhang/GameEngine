#include "Singleton.h"

#include "Profiler.h"

void singletonCreateDestroy( bool isInit);

void initAllSingletons()
{
	singletonCreateDestroy(true);
}

void destroyAllSingletons()
{
	singletonCreateDestroy(false);
}

void singletonCreateDestroy( bool isInit)
{
	{ 
		if (isInit)
		{
			Singleton<Profiler>::getInstance().init();
            // Put any additional init code here
		}
		else
		{
            // Put any additional destroy code here
			Singleton<Profiler>::getInstance().destroy();
		}
	} 

	{
		if (isInit)
		{
		}
		else
		{
		}
	}  
}
