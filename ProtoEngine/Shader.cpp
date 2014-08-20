#include "Shader.h"
#include "Material.h"

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

Shader::Shader( ID3DX11Effect* fx, ShaderVarTagDefinition* tagDef)
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

void Shader::setMaterial( std::vector<MaterialAttributeInterface*>& attrs )
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

void Shader::setMaterial( MaterialInterface* material )
{
    for (uint32 i = 0; i < material->mAttributes.size(); ++i)
    {
        trySet(material->mAttributes[i]);
    }
}

bool Shader::trySet( MaterialAttributeInterface* matAttrInterface )
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

