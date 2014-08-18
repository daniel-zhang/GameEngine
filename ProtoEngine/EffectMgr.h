#ifndef EFFECT_MGR_H
#define EFFECT_MGR_H

#include <string>
#include <vector>
#include "Singleton.h"
#include "d3dx11effect.h"
#include "Shader.h"

class RenderInterface;

class EffectMgr : public Singleton<EffectMgr>
{
public:
    EffectMgr();
    ~EffectMgr();
        
    bool init(RenderInterface* ri);
    bool destroy();

    Shader* getByName(const char* effectName);
    Shader* getByIndex();

protected:
    std::string mDir;
    std::string mExtension;

    ShaderVarTagDefinition mTagDefinition;
    std::vector<Shader*> mEffectColletion;
};



#endif