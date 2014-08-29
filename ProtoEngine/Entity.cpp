#include "Entity.h"
#include "Mesh.h"

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
