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



void ShaderParameter::set( MaterialAttributeInterface* IMatAttr)
{
    IMatAttr->sync(this);
}

Shader::Shader( ID3DX11Effect* fx, ShaderVarTagDefinition* tagDef)
{
    mShaderVarTagDef = tagDef;
    mFx = fx;
    mFx->GetDesc(&mFxDesc);
}

bool Shader::init()
{
    bool ret = true;
    // Bind global shader variables
    for (uint32 i = 0; i < mFxDesc.GlobalVariables; ++i)
    {
        ID3DX11EffectVariable* var = mFx->GetVariableByIndex(i);
        ID3DX11EffectVariable* annotation = var->GetAnnotationByName("tag");
        // Shader variable has <string tag="...">?
        if (annotation->IsValid())
        {
            const char* tmp; 
            annotation->AsString()->GetString(&tmp);
            ShaderVarTag* binding = mShaderVarTagDef->query(std::string(tmp));
            // <string tag="..."> is defined?
            if (binding)
            {
                //mParameters.push_back(ShaderParameter(var, binding->mTagEnum));
                mParamMap.insert(std::pair<EnumShaderVarTag, ShaderParameter>(binding->mTagEnum, ShaderParameter(var, binding->mTagEnum)));
            }
            // Shader variable's tag is not defined
            else
            {
                mTagsNotDefined.push_back(var);
                ret = false;
            }
        }
        // Shader variable has no <string tag="...">
        else
        {
            mTagsMissing.push_back(var);
            ret = false;
        }
    }
    return ret;
}

void Shader::apply( std::vector<MaterialAttributeInterface*>& attrs )
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

bool Shader::trySet( MaterialAttributeInterface* matAttrInterface )
{
    std::map<EnumShaderVarTag, ShaderParameter>::iterator iter;
    iter = mParamMap.find(matAttrInterface->mTag);
    if (iter != mParamMap.end())
    {
        (iter->second).set(matAttrInterface);
        return true;
    }
    return false;
}

