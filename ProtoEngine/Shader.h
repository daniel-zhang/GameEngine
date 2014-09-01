#ifndef EFFECT_WRAPPER_H
#define EFFECT_WRAPPER_H

#include <string>
#include <vector>
#include <map>

#include "typedefs.h"
#include "d3dx11effect.h"
#include "ShaderEnum.h"

class ShaderDataReference;
class AttrReferenceInterface;
class RenderInterface;
//
// Define all variables in all shaders
//
/*
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
class MetaEffect
{
public:
    MetaEffect(ID3DX11Effect* fx, ShaderVarTagDefinition* tagDefs);
    ~MetaEffect(){}

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
    /*
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
*/

//
// Version 2
//

class ShaderParameter
{
public:
    ShaderParameter(ID3DX11EffectVariable* var, NativeEnum_ShaderVarTag tag);

    void assign_from(AttrReferenceInterface* attrRef);

    NativeEnum_ShaderVarTag mTag;
    ID3DX11EffectVariable* mVar;
};

class ShaderEffect 
{
public:
    ShaderEffect();
    bool init(RenderInterface* ri, ID3DX11Effect* inFx, SmartEnum_ShaderVarTag* inTagDefintion);
    void clear();
    void setShaderData(ShaderDataReference& shaderData);
    void apply(RenderInterface* ri);
    void test_verify();

protected:
    bool try_assign_from(AttrReferenceInterface* attrRef);

public:
    ID3DX11Effect* mFx;
    ID3DX11EffectTechnique* mMainTech;
    std::string mVertexFormatString;
    ID3D11InputLayout* mInputLayout;

    SmartEnum_ShaderVarTag* mTagDefinition;
    typedef std::map<NativeEnum_ShaderVarTag, ShaderParameter> TParameterMap;
    TParameterMap mValidParams;

    // For debug purpose
    std::vector<ShaderParameter> mTagsDuplicated;
    std::vector<ID3DX11EffectVariable*> mTagsMissing;
    std::vector<std::string> mTagsNotDefined;
};

/*
class ShaderInterface
{
public:
    ShaderInterface();
    ShaderInterface(std::string& inEffectName, std::string inTechName, uint32 inPassIndex );

    void init(std::string& inEffectName, std::string inTechName, uint32 inPassIndex );
    bool isValid();
    void clear();

    //void setMaterial(MaterialInterface* material);
    void apply(RenderInterface* ri);

    ShaderEffect* getOwningEffect();

protected:
    std::string mEffectName;
    std::string mTechName;
    uint32 mPassIndex;
    bool mIsValid;

    ShaderEffect* mEffect;
    ID3DX11EffectTechnique* mTech;
    ID3DX11EffectPass* mPass;
};
*/

/*
Use-case:

ShaderLinkage:
    uint32 effect_index
    uint32 tech_index
    uint32 pass_index

material:
    EffectInstance* effect;
    TechInstance* tech;
    PasstInstance* pass;

material.setShaderLinkage(effect_name, tech_name, pass_name):
    
for(...)
    material.updateAttributes(...);
    material.apply():
        fxInstace.getPass(pShaderLinkage)->


class MetaEffect;
class MetaTechnique;
class MetaPass;
*/


#endif