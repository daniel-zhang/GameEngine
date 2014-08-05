#ifndef SINGLETON_H
#define SINGLETON_H

// An Ogre style singleton which requires explicit init/destroy
// Restrictions:
// content class provides a default ctor

template<class T>
class Singleton
{
public:
	static void CreateSingleton()
	{
		instance = new T();
	}
	static void DestroySingleton()
	{
		delete instance;
		instance = 0;
	}

	static T* instance;
};

void initAllSingletons();
void destroyAllSingletons();

    
#endif