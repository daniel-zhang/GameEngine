#include "Material.h"
#include "RenderInterface.h"
#include "Entity.h"
#include "ShaderEnum.h"
#include "Shader.h"
#include "EffectMgr.h"

////////////////////////////////////////////////////////
DefaultMaterial::DefaultMaterial(std::string& shaderName) : mDefaultShaderName(shaderName)
{
    mShader = NULL;
    determinShaderToLink();
    buildDefaultAttributes();
    buildMaterialShaderDataReference();
}
DefaultMaterial::~DefaultMaterial()
{
}

//void DefaultMaterial::apply( RenderInterface* ri, Entity* entity )
//{
    /*
    TODO: replace with XMMatrixTransformation:
        XMINLINE XMMATRIX XMMatrixTransformation
        (
        FXMVECTOR ScalingOrigin, 
        FXMVECTOR ScalingOrientationQuaternion, 
        FXMVECTOR Scaling, 
        CXMVECTOR RotationOrigin, 
        CXMVECTOR RotationQuaternion, 
        CXMVECTOR Translation
    ) */
    /*
    const XMFLOAT3& translation = entity->getTranslation();
    const XMFLOAT3& rotation = entity->getRotation();
    const XMFLOAT3& scaling = entity->getScaling();
    // Rotation is for now ignored
    //XMStoreFloat4x4( &mLocalToWorld, XMMatrixMultiply( XMMatrixTranslation(translation.x, translation.y, translation.z ), XMMatrixScaling(scaling.x, scaling.y, scaling.z )) );
    */

    // TODO: set a real map
//    mDiffuseMap = NULL;

    //mShader->setMaterial(this);
//}

void DefaultMaterial::determinShaderToLink()
{
    mShader = Singleton<EffectMgr>::getInstance().getEffect(mDefaultShaderName);
}

void DefaultMaterial::buildDefaultAttributes()
{
    //ZeroMemory(&mLocalToWorld, sizeof(XMFLOAT4X4));
    mDiffuseMap = NULL;

    mMeshMaterial.ambient  = XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
    mMeshMaterial.diffuse  = XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
    mMeshMaterial.specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

    // Standalone render states
}

void DefaultMaterial::buildMaterialShaderDataReference()
{
    mMaterialShaderData.add<e_texture>(mDiffuseMap);
    mMaterialShaderData.add<e_mesh_material>(mMeshMaterial);
}

