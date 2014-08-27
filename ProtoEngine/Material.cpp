#include "Material.h"
#include "RenderInterface.h"
#include "Entity.h"
#include "ShaderEnum.h"
#include "Shader.h"
#include "EffectMgr.h"

MaterialInterface::MaterialInterface(std::string& inShaderName)
{
    mShader = new ShaderInterface();
    // TODO
    mShader->init(std::string(), std::string(), 0 );
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

    if (mShader != NULL)
    {
        delete mShader;
        mShader = NULL;
    }
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
    const XMFLOAT3& translation = entity->getTranslation();
    const XMFLOAT3& rotation = entity->getRotation();
    const XMFLOAT3& scaling = entity->getScaling();

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
    mpLocalToWorld = new MaterialAttr<XMFLOAT4X4>(e_local_to_world);
    mpDiffuseMap = new MaterialAttr<ID3D11ShaderResourceView>(e_texture);
    // TODO: mpDiffuseMap->pData = TextureMgr::someTexture()
    mpObjMaterial = new MaterialAttr<MeshMaterial>(e_plain_material);
    mpObjMaterial->pData->Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
    mpObjMaterial->pData->Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 2.0f);
    mpObjMaterial->pData->Specular= XMFLOAT4(0.3f, 0.3f, 0.3f, 3.0f);

    mAttributes.push_back(mpLocalToWorld);
    mAttributes.push_back(mpDiffuseMap);
    mAttributes.push_back(mpObjMaterial);

    // Standalone render states
}
