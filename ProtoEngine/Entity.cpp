#include "Entity.h"
#include "Scene.h"
#include "Mesh.h"
#include "ProtoMath.h"

void IControllable::walk( float d )
{
    XMVECTOR s = XMVectorReplicate(d);
    XMVECTOR l = XMLoadFloat3(mRotation.getFrontVector());
    XMVECTOR p = XMLoadFloat3(&mTranslation);
    XMStoreFloat3(&mTranslation, XMVectorMultiplyAdd(s, l, p));
}

void IControllable::strafe( float d )
{
    XMVECTOR s = XMVectorReplicate(d);
    XMVECTOR r = XMLoadFloat3(mRotation.getRightVector());
    XMVECTOR p = XMLoadFloat3(&mTranslation);
    XMStoreFloat3(&mTranslation, XMVectorMultiplyAdd(s, r, p));
}

void IControllable::pitch( float d )
{
    mRotation.pitch(d);
}

void IControllable::yaw( float d )
{
    mRotation.yaw(d);
}

void IControllable::roll( float d )
{
    mRotation.roll(d);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

Entity::Entity()
{
    update(0.f);
    buildEntityShaderData();
    mSceneRef = NULL;
    mMeshRef = NULL;
}

void Entity::update( float delta )
{
    XMMATRIX local2world = XMMatrixMultiply( XMMatrixTranslation(mTranslation.x, mTranslation.y, mTranslation.z), XMMatrixScaling(mScaling.x, mScaling.y, mScaling.z) );
    XMMATRIX inv_tran_local2world = ProtoMath::InverseTranspose(local2world);
    XMStoreFloat4x4( &mLocalToWorld, local2world );
    XMStoreFloat4x4( &mInvertTranpose_LocalToWorld, inv_tran_local2world);

    // No rotation 
    // TODO: replace with XMMatrixTransformation
}

void Entity::drawSelf( RenderInterface* ri )
{
    // Before draw mesh

    // Draw mesh
    mMeshRef->drawSelf(ri, this);

    // After draw mesh
}

void Entity::buildEntityShaderData()
{
    mEntityShaderData.add<e_local_to_world>(mLocalToWorld);
    mEntityShaderData.add<e_world_inv_transpose>(mInvertTranpose_LocalToWorld);
}

ShaderDataReference& Entity::getEntityShaderData()
{
    return mEntityShaderData;
}

void Entity::setSceneRef( Scene* inScene )
{
    mSceneRef = inScene;
}

Scene* Entity::getSceneRef()
{
    return mSceneRef;
}

void Entity::attach( Mesh* mesh, RenderInterface* ri )
{
    mMeshRef = mesh; 
    mMeshRef->onAttachedToEntity(ri);
}

