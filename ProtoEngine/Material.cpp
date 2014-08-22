#include "Material.h"
#include "RenderInterface.h"
#include "Entity.h"
#include "ShaderEnum.h"
#include "Shader.h"
#include "EffectMgr.h"

MaterialInterface::MaterialInterface(std::string& inShaderName)
{
    mShader = Singleton<ShaderMgr>::getInstance().getShaderByName(inShaderName);
}

MaterialInterface::~MaterialInterface()
{
    for (uint32 i = 0; i < mAttributes.size(); ++i)
    {
        if (mAttributes[i])
        {
            delete mAttributes[i];
        }
    }
    mAttributes.clear();
}

DefaultMaterial::DefaultMaterial( std::string& shaderName ) : MaterialInterface(shaderName)
{
    setDefault();
}


void DefaultMaterial::apply( RenderInterface* ri, Entity* entity )
{
    //
    // update material attributes
    //
    XMFLOAT3& translation = entity->mTranslation;
    XMFLOAT3& rotation = entity->mRotation;
    XMFLOAT3& scaling = entity->mScaling;

    // TODO: replace with XMMatrixTransformation
    XMStoreFloat4x4( 
        mpLocalToWorld->pData, 
        XMMatrixMultiply( XMMatrixTranslation(translation.x, translation.y, translation.z ), XMMatrixScaling(scaling.x, scaling.y, scaling.z ))
        );

    // TODO: set a real map
    mpDiffuseMap->pData = NULL;

    //
    // Select shader(effect + tech) and sync
    //
    mShader->setMaterial(this);

    //mShader->apply_default(ri);
}

void DefaultMaterial::setDefault()
{
    // Per object material attribute
    mpLocalToWorld = new MaterialAttr<XMFLOAT4X4>(local_to_world);
    mpDiffuseMap = new MaterialAttr<ID3D11ShaderResourceView>(texture);
    // TODO: mpDiffuseMap->pData = TextureMgr::someTexture()
    mpObjMaterial = new MaterialAttr<MeshMaterial>(plain_material);
    mpObjMaterial->pData->Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
    mpObjMaterial->pData->Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 2.0f);
    mpObjMaterial->pData->Specular= XMFLOAT4(0.3f, 0.3f, 0.3f, 3.0f);

    mAttributes.push_back(mpLocalToWorld);
    mAttributes.push_back(mpDiffuseMap);
    mAttributes.push_back(mpObjMaterial);

    // Standalone render states
}
