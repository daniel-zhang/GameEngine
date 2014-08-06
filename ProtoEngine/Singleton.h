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
3. Though lazy initialization is used, init/destroy order is explicitly specified in:
    void initAllSingletons();
    void destroyAllSingletons();

Remarks:
   If init order becomes a concern, refer to this SO post:
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

void initAllSingletons();
void destroyAllSingletons();

#endif