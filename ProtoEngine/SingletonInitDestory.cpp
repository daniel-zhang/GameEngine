#include "Singleton.h"

#include "Profiler.h"

template<class T> T* Singleton<T>::instance = 0;

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
			Singleton<Profiler>::CreateSingleton();
			Singleton<Profiler>::instance->init();
		}
		else
		{
			Singleton<Profiler>::instance->destroy();
			Singleton<Profiler>::DestroySingleton();
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
