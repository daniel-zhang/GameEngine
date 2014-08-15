#ifndef SINGLETON_H
#define SINGLETON_H

/*
Usage:
    class SomeMgr : public Singleton<SomeMgr> {...};
    Singleton<SomeMgr>::getInstance().init(...);
    Singleton<SomeMgr>::getInstance().do_something();

Requirements:
1. SomeMgr must have a default ctor 
2. SomeMgr can have a init/destroy pair
3. Lazy-creation + explicit init/destroy
    void initAllSingletons();
    void destroyAllSingletons();

Remarks:
   http://stackoverflow.com/questions/335369/finding-c-static-initialization-order-problems/335746#335746
*/

template<typename T>
class Singleton
{
public:
    Singleton() {}
    ~Singleton(){}
    static T& getInstance()
    {
        static T instance;
        return instance;
    }
private:
    Singleton(const Singleton<T>& rhs);
    Singleton& operator=(const Singleton<T>& rhs);
};

/*
Phase 1 singletons - independent of graphics pipeline 
    e.g. profiler, config mgr, ...
Phase 2 singletons - dependent of render device and context
    e.g. effect mgr, texture mgr, ...
*/
void initPhaseOneSingletons();
void clearPhaseOneSingletons();

void initPhaseTwoSingletons();
void clearPhaseTwoSingletons();

#endif