#ifndef EFFECT_MGR_H
#define EFFECT_MGR_H

#include <string>
#include <vector>
#include <map>
#include "reference.h"
#include "Singleton.h"
#include "Shader.h"
#include "GraphicBuffer.h"

class RenderInterface;

/*
class EffectMgr : public Singleton<EffectMgr>
{
public:
    EffectMgr();
    ~EffectMgr();
        
    bool init(RenderInterface* ri);
    bool destroy();

    Effect* getEffect(std::string inEffectName);
    MetaEffect* getByName(const char* effectName);
    MetaEffect* getByIndex();

protected:
    std::string mDir;
    std::string mExtension;

    SmartEnum_ShaderVarTag mTagDefinition;
    std::vector<MetaEffect*> mEffectColletion;
};
*/

class EffectMgr: public Singleton<EffectMgr>
{
public:
    EffectMgr();
    ~EffectMgr();

    bool init(RenderInterface* ri);
    bool destroy();

    ShaderEffect* getEffect(std::string& inEffectName);
    

protected:
    void buildEffectNameMap();
    bool effectNameToIndex(const std::string& inEffectName, uint32& outIndex);
    typedef std::map<std::string, uint32> TEffectNameMap;
    TEffectNameMap mEffectNameMap;
    std::vector<std::string> mEffectNames;

protected:
    SmartEnum_ShaderVarTag mTagDefinition;
    std::vector<ShaderEffect*> mEffects;
};

void effect_mgr_test();

#endif