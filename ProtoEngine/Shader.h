#ifndef EFFECT_WRAPPER_H
#define EFFECT_WRAPPER_H

#include "typedefs.h"
#include "d3dx11effect.h"
#include "ShaderEnum.h"
#include <string>
#include <vector>
#include <map>

class EffectInspector
{
public:
    EffectInspector(){mFX = NULL;}
    EffectInspector(ID3DX11Effect* fx):mFX(fx){}

    void go()
    {
        if(mFX == NULL)
            return;
        /*
        UINT    ConstantBuffers;        // Number of constant buffers in this effect
        UINT    GlobalVariables;        // Number of global variables in this effect
        UINT    InterfaceVariables;     // Number of global interfaces in this effect
        UINT    Techniques;             // Number of techniques in this effect
        UINT    Groups;                 // Number of groups in this effect
        */
        D3DX11_EFFECT_DESC desc;
        mFX->GetDesc(&desc);

        for (uint32  i  = 0; i < desc.ConstantBuffers; ++i)
        {
            mConstBuffers.push_back(mFX->GetConstantBufferByIndex(i));
            D3DX11_EFFECT_VARIABLE_DESC cbufferDesc;
            mConstBuffers[i]->GetDesc(&cbufferDesc);
            mConstBufferDescs.push_back(cbufferDesc);
        }

        for (uint32 i = 0; i < desc.GlobalVariables; ++i)
        {
            mGlobalVars.push_back(mFX->GetVariableByIndex(i));

            D3DX11_EFFECT_VARIABLE_DESC varDesc;
            mGlobalVars[i]->GetDesc(&varDesc);
            mGlobalVarDescs.push_back(varDesc);
        }

        for (uint32  i  = 0; i < desc.Techniques; ++i)
        {
            mTechs.push_back(mFX->GetTechniqueByIndex(i));
            D3DX11_TECHNIQUE_DESC techDesc;
            mTechs[i]->GetDesc(&techDesc);
            mTechDescs.push_back(techDesc);
            for(uint32 j = 0; j < techDesc.Passes; ++j)
            {
                mPasses.push_back(mTechs[i]->GetPassByIndex(j));
                D3DX11_PASS_DESC passDesc;
                mPasses[mPasses.size() - 1]->GetDesc(&passDesc);
                mPassDescs.push_back(passDesc);
            }
        }
    }

protected:
    ID3DX11Effect* mFX;
    std::vector<ID3DX11EffectVariable*> mGlobalVars;
    std::vector<D3DX11_EFFECT_VARIABLE_DESC> mGlobalVarDescs;

    std::vector<ID3DX11EffectConstantBuffer*> mConstBuffers;
    std::vector<D3DX11_EFFECT_VARIABLE_DESC> mConstBufferDescs;

    std::vector<ID3DX11EffectTechnique*> mTechs;
    std::vector<D3DX11_TECHNIQUE_DESC> mTechDescs;

    std::vector<ID3DX11EffectPass*> mPasses;
    std::vector<D3DX11_PASS_DESC> mPassDescs;
};


class MaterialAttributeInterface;
class MaterialInterface;
class RenderInterface;

//
// Define all variables in all shaders
//
struct ShaderVarTag
{
    ShaderVarTag(EnumShaderVarTag tagEnum, const char* tagStr) :mTagEnum(tagEnum), mTagStr(tagStr) { }
    EnumShaderVarTag mTagEnum;
    std::string mTagStr;
};

class ShaderVarTagDefinition
{
public:
    ShaderVarTagDefinition();
    ShaderVarTag* query(std::string& tagStr);

protected:
    std::vector<ShaderVarTag> mDefinitions;
};

class ShaderParameter
{
public:
    ShaderParameter(ID3DX11EffectVariable* inVar, EnumShaderVarTag varTag):mVar(inVar), mVarTag(varTag) { }

    ID3DX11EffectVariable* mVar;
    EnumShaderVarTag mVarTag;
    void updateFrom(MaterialAttributeInterface* IMatAttr);
};

class Shader
{
public:
    Shader(ID3DX11Effect* fx, ShaderVarTagDefinition* tagDefs);
    ~Shader(){}

    void setMaterial(std::vector<MaterialAttributeInterface*>& attrs);
    void setMaterial(MaterialInterface* material);
    void setOthers();

    void apply_default(RenderInterface* ri);
    void apply(RenderInterface* ri, uint32 techIndex);

protected:
    bool trySet(MaterialAttributeInterface* IMatAttr);

    // Shader Linkage
public:
    ID3DX11Effect* mFx;
    ID3DX11EffectTechnique* mDefaultTech;
    std::vector<ID3DX11EffectTechnique*> mTechs;
    uint32 mActiveTechIndex;

    // Parameters
public:
    ShaderVarTagDefinition* mShaderVarTagDef;
    typedef std::map<EnumShaderVarTag, ShaderParameter> ShaderParamMap;
    std::map<EnumShaderVarTag, ShaderParameter> mParamMap;
    
public:
    // For debug purpose
    std::vector<ID3DX11EffectVariable*> mTagsMissing;
    std::vector<ID3DX11EffectVariable*> mTagsNotDefined;
};


#endif