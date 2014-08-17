#ifndef EFFECT_MGR_H
#define EFFECT_MGR_H

#include <string>
#include <vector>
#include "Singleton.h"
#include "d3dx11effect.h"

class Effect;
class RenderInterface;

class EffectMgr : public Singleton<EffectMgr>
{
public:
    EffectMgr();
    ~EffectMgr();
        
    bool init(RenderInterface* ri);
    bool destroy();

    Effect* getByName(const char* effectName);
    Effect* getByIndex();

protected:
    std::string mDir;
    std::string mExtension;
    std::vector<Effect*> mEffectColletion;
};



#endif