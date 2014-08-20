#ifndef EFFECT_MGR_H
#define EFFECT_MGR_H

#include <string>
#include <vector>
#include <map>
#include "reference.h"
#include "Singleton.h"
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


class ShaderMgr: public Singleton<ShaderMgr>
{
public:
    ShaderMgr();
    ~ShaderMgr();

    bool init(RenderInterface* ri);
    bool destroy();

    Shader* getShaderByName(std::string& shaderName);

protected:
    void buildShaderNameMap();
    bool shaderNameToIndex(const std::string& inShaderName, uint32& outIndex);
    bool loadShader();
    typedef std::map<std::string, uint32> ShaderNameMap;
    ShaderNameMap mShaderNameMap;
    std::vector<std::string> mShaderNames;

protected:
    ShaderVarTagDefinition mTagDefinition;
    std::vector<Shader*> mShaders;
};

void shader_mgr_test();

#endif