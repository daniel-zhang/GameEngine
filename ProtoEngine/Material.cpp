#include "Material.h"
#include "RenderInterface.h"
#include "Entity.h"
#include "ShaderEnum.h"
#include "Shader.h"
#include "EffectMgr.h"

////////////////////////////////////////////////////////
DefaultMaterial::DefaultMaterial(std::string shaderName) : mDefaultShaderName(shaderName)
{
    mShader = NULL;
    buildDefaultAttributes();
    buildShaderDataReference();
}
DefaultMaterial::~DefaultMaterial()
{
    if (mShader != NULL) { delete mShader; mShader = NULL; }
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
    mMeshMaterial.ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
    mMeshMaterial.diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 2.0f);
    mMeshMaterial.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 3.0f);
    // Standalone render states
}

void DefaultMaterial::buildShaderDataReference()
{
    mShaderData.add<e_texture>(mDiffuseMap);
    mShaderData.add<e_plain_material>(mMeshMaterial);
}

