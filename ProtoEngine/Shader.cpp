#include "Shader.h"
#include "Material.h"
#include "EffectMgr.h"
#include "GraphicBuffer.h"
#include "ShaderDataReference.h"
/*
#define DEF_SHADER_VAR_TAG(e) mDefinitions.push_back(ShaderVarTag(e, #e))

ShaderVarTagDefinition::ShaderVarTagDefinition()
{
    mDefinitions.clear();
    DEF_SHADER_VAR_TAG( cam_pos );
    DEF_SHADER_VAR_TAG( local_to_world );
    DEF_SHADER_VAR_TAG( world_to_view );
    DEF_SHADER_VAR_TAG( view_to_proj );
    DEF_SHADER_VAR_TAG( world_view_proj );
    DEF_SHADER_VAR_TAG( tex_transform );
    DEF_SHADER_VAR_TAG( world_inv_transpose );
    DEF_SHADER_VAR_TAG( plain_material );
    DEF_SHADER_VAR_TAG( texture );
    DEF_SHADER_VAR_TAG( anisotropic_sampler );
}

ShaderVarTag* ShaderVarTagDefinition::query( std::string& tagStr )
{
    for (uint32 i = 0; i < mDefinitions.size(); ++i)
    {
        if (mDefinitions[i].mTagStr.compare(tagStr) == 0)
        {
            return &mDefinitions[i];
        }
    }
    return NULL;
}



void ShaderParameter::updateFrom( MaterialAttributeInterface* IMatAttr)
{
    IMatAttr->sync(this);
}

MetaEffect::MetaEffect( ID3DX11Effect* fx, ShaderVarTagDefinition* tagDef)
{
    mShaderVarTagDef = tagDef;
    mFx = fx;

    D3DX11_EFFECT_DESC fxDesc;
    mFx->GetDesc(&fxDesc);

    // parse annotation of global shader vars
    for (uint32 i = 0; i < fxDesc.GlobalVariables; ++i )
    {
        ID3DX11EffectVariable* var = mFx->GetVariableByIndex(i);
        ID3DX11EffectVariable* varTag = var->GetAnnotationByName("tag");
        if (varTag->IsValid())
        {
            const char* tagstr;
            varTag->AsString()->GetString(&tagstr);
            ShaderVarTag* tag = mShaderVarTagDef->query(std::string(tagstr));
            if (tag != NULL)
            {
                mParamMap.insert(std::pair<EnumShaderVarTag, ShaderParameter>(tag->mTagEnum, ShaderParameter(var, tag->mTagEnum)));
            }
            else
            {
                mTagsNotDefined.push_back(var);
            }
        }
        else
        {
            mTagsMissing.push_back(var);
        }
    }

    // parse annotation of techniques
    for (uint32 i = 0; i < fxDesc.Techniques; ++i)
    {
        ID3DX11EffectTechnique* tech = mFx->GetTechniqueByIndex(i);
        if (tech->IsValid())
        {
            mTechs.push_back(tech);
        }
        ID3DX11EffectVariable* annotation = tech->GetAnnotationByName("default_tech");
        if (annotation->IsValid())
        { 
            BOOL isDefaultTech = FALSE;
            annotation->AsScalar()->GetBool(&isDefaultTech);
            if (isDefaultTech)
            {
                mDefaultTech = tech;
            }
        }
    }


}

void MetaEffect::setMaterial( std::vector<MaterialAttributeInterface*>& attrs )
{
    for (uint32 i = 0; i < attrs.size(); ++i)
    {
        bool val = trySet(attrs[i]);
        if (val)
        {
            // good 
            ;
        }
        else
        {
            // not good
            ;
        }
    }
}

void MetaEffect::setMaterial( MaterialInterface* material )
{
    for (uint32 i = 0; i < material->mAttributes.size(); ++i)
    {
        trySet(material->mAttributes[i]);
    }
}

bool MetaEffect::trySet( MaterialAttributeInterface* matAttrInterface )
{
    std::map<EnumShaderVarTag, ShaderParameter>::iterator iter;
    iter = mParamMap.find(matAttrInterface->mTag);
    if (iter != mParamMap.end())
    {
        (iter->second).updateFrom(matAttrInterface);
        return true;
    }
    return false;
}
*/

//
//
//

ShaderEffect::ShaderEffect()
{
    clear();
}

bool ShaderEffect::init( RenderInterface* ri, ID3DX11Effect* inFx, SmartEnum_ShaderVarTag* inTagDefintion )
{
    if (!inFx->IsValid())
    {
        return false;// throw sth
    }
    mFx = inFx;
    mTagDefinition = inTagDefintion;

    D3DX11_EFFECT_DESC fxDesc;
    mFx->GetDesc(&fxDesc);
    // Parse annotation of global shader vars
    for (uint32 i = 0; i < fxDesc.GlobalVariables; ++i )
    {
        ID3DX11EffectVariable* var = mFx->GetVariableByIndex(i);
        ID3DX11EffectVariable* tag = var->GetAnnotationByName("tag");
        if (!tag->IsValid())
        {
            // Tag annotation is missing
            mTagsMissing.push_back(var);
        }
        else
        {
            const char* lpcsTag;
            tag->AsString()->GetString(&lpcsTag);
            std::string tagstr(lpcsTag);
            NativeEnum_ShaderVarTag eTag = mTagDefinition->toEnum(tagstr);
            if (eTag != (NativeEnum_ShaderVarTag)(-1))
            {
                ShaderParameter sp(var, eTag);
                TParameterMap::const_iterator iter = mValidParams.find(eTag);
                if (iter != mValidParams.end())
                    // Duplicated tags
                    mTagsDuplicated.push_back(sp);
                else
                    mValidParams.insert(std::pair<NativeEnum_ShaderVarTag, ShaderParameter>(eTag, sp));
            }
            else
            {
                // Tag is not defined
                mTagsNotDefined.push_back(tagstr);
            }
        }
    }
    // Parse annotation of technique
    ID3DX11EffectTechnique* tech = mFx->GetTechniqueByIndex(0);
    mMainTech = tech;
    if(tech->IsValid())
    {
        ID3DX11EffectVariable* vertFormat = tech->GetAnnotationByName("VertexFormat");
        if (vertFormat->IsValid())
        {
            const char* lpcsVF;
            vertFormat->AsString()->GetString(&lpcsVF);
            mVertexFormatString = lpcsVF;

            D3DX11_PASS_DESC passDesc;
            tech->GetPassByIndex(0)->GetDesc(&passDesc);

            if (mVertexFormatString.compare("PosNormalTex") == 0)
            {
                d3d_check(ri->mDevice->CreateInputLayout( 
                    InputLayoutDesc<e_pos_normal_tex>::format_desc,
                    InputLayoutDesc<e_pos_normal_tex>::desc_num,
                    passDesc.pIAInputSignature, 
                    passDesc.IAInputSignatureSize, 
                    &mInputLayout));
            }
            else if (mVertexFormatString.compare("PosNormalTanTex") == 0)
            {
                d3d_check(ri->mDevice->CreateInputLayout( 
                    InputLayoutDesc<e_pos_normal_tan_tex>::format_desc,
                    InputLayoutDesc<e_pos_normal_tan_tex>::desc_num,
                    passDesc.pIAInputSignature, 
                    passDesc.IAInputSignatureSize, 
                    &mInputLayout));
            }
            else
            {
                // invalid <string VertexFormat="...";>
            }
        }
    }
    return true;
}

void ShaderEffect::clear()
{
    mFx = NULL;
    mMainTech = NULL;
    mTagDefinition = NULL;

    mValidParams.clear();

    mTagsDuplicated.clear();
    mTagsMissing.clear();
    mTagsNotDefined.clear();
}

bool ShaderEffect::try_assign_from(AttrReferenceInterface* attrRef)
{
    this;
    TParameterMap::iterator iter;
    iter = mValidParams.find(attrRef->tag_enum());
    if (iter != mValidParams.end())
    {
        attrRef->assign_to(&(iter->second));
        return true;
    }
    return false;
}

void ShaderEffect::setShaderData( ShaderDataReference& shaderData)
{
    this;
    uint32 targetSize = shaderData.mRefs.size();
    for (uint32 i = 0;i < targetSize; ++i)
    {
        try_assign_from(shaderData.mRefs[i]);
    }
}

void ShaderEffect::apply( RenderInterface* ri )
{
    mMainTech->GetPassByIndex(0)->Apply(0, ri->mCtx);
}

void ShaderEffect::test_verify()
{
    TParameterMap::iterator iter;
    for (iter = mValidParams.begin(); iter != mValidParams.end(); ++iter)
    {
        ShaderParameter* sp = &(iter->second);
        switch(sp->mTag)
        {
        case e_cam_pos:
            {
                XMFLOAT3 data0;
                sp->mVar->GetRawValue(reinterpret_cast<void*>(&data0), 0, sizeof(XMFLOAT3));
            }
            break;

        case e_local_to_world:
            {
                XMFLOAT4X4 data1;
                sp->mVar->GetRawValue(reinterpret_cast<void*>(&data1), 0, sizeof(XMFLOAT4X4));
            }
            break;

        case e_world_to_view:
            {
                XMFLOAT4X4 data2;
                sp->mVar->GetRawValue(reinterpret_cast<void*>(&data2), 0, sizeof(XMFLOAT4X4));
            }
            break;

        case e_view_to_proj:
            {
                XMFLOAT4X4 data3;
                sp->mVar->GetRawValue(reinterpret_cast<void*>(&data3), 0, sizeof(XMFLOAT4X4));
            }
            break;
        }
    }
}

ShaderParameter::ShaderParameter( ID3DX11EffectVariable* var, NativeEnum_ShaderVarTag tag ) 
    :mVar(var), mTag(tag) { }

void ShaderParameter::assign_from(AttrReferenceInterface* attrRef)
{
    attrRef->assign_to(this);
}
/*
ShaderInterface::ShaderInterface()
{
    clear();
}

ShaderInterface::ShaderInterface( std::string& inEffectName, std::string inTechName, uint32 inPassIndex )
{
    clear();
    init(inEffectName, inTechName, inPassIndex );
}

void ShaderInterface::init( std::string& inEffectName, std::string inTechName, uint32 inPassIndex )
{
    mEffectName = inEffectName;
    mTechName = inTechName;
    mPassIndex = inPassIndex;

    mEffect = Singleton<EffectMgr>::getInstance().getEffect(mEffectName);
    if (!mEffect)
    {
        mIsValid = false;
    }
    else
    {
        mTech = mEffect->mFx->GetTechniqueByName(mTechName.c_str());
        mPass = mTech->GetPassByIndex(mPassIndex);
        if (mTech->IsValid() && mPass->IsValid())
            mIsValid = true;
        else
            mIsValid = false;
    }
}

bool ShaderInterface::isValid()
{
    return mIsValid;
}

void ShaderInterface::clear()
{
    mEffectName.clear(); 
    mTechName.clear();
    mPassIndex = 0;
    mEffect = NULL;
    mTech = NULL;
    mPass = NULL;

    mIsValid = false;
}
void ShaderInterface::setMaterial( MaterialInterface* inMaterial)
{
    for (uint32 i = 0; i < inMaterial->mAttributes.size(); ++i)
    {
        bool succ = mEffect->trySetFrom(inMaterial->mAttributes[i]);
        if (succ)
        {

        }
        else
        {

        }
    }
}
void ShaderInterface::apply( RenderInterface* ri )
{
    // TODO
}
ShaderEffect* ShaderInterface::getOwningEffect()
{
    return mEffect;
}

*/


