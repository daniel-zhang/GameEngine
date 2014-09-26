#include "Camera.h"

Frustum::Frustum(ECoordSystem e) : mCoordSys(e)
{
    mNearDepth   = 0.f;
    mFarDepth    = 0.f;
    mAspect      = 0.0f;
    mVerticalFOV = 0.f;

    mNearWindowHeight = 0.f;
    mNearWindowWidth  = 0.f;
    mFarWindowHeight  = 0.f;
    mFarWindowWidth   = 0.f;

    ZeroMemory(&mProj, sizeof(XMFLOAT4X4));
}

void Frustum::setAll( float aspect, float vFOV, float nearDepth, float farDepth )
{
    mNearDepth = nearDepth;
    mFarDepth = farDepth;
    mAspect = aspect;
    mVerticalFOV = vFOV;
    sync();
}

void Frustum::setAspect( float aspect )
{
    mAspect = aspect;
    sync();
}

void Frustum::setFOV( float vFOV )
{
    mVerticalFOV = vFOV;
    sync();
}

void Frustum::sync()
{
    syncNearFarWindowSize();
    syncProj();
}

void Frustum::syncNearFarWindowSize()
{
    mNearWindowHeight = 2.0f * mNearDepth * tanf( 0.5f*mVerticalFOV);
    mFarWindowHeight  = 2.0f * mFarDepth * tanf( 0.5f*mVerticalFOV);
    mNearWindowWidth  = mAspect * mNearWindowHeight;
    mFarWindowWidth   = mAspect * mFarWindowHeight;
}

void Frustum::syncProj()
{
    XMMATRIX P;
    if (IsLeftHanded(mCoordSys))
        P = XMMatrixPerspectiveFovLH(mVerticalFOV, mAspect, mNearDepth, mFarDepth);
    else
        P = XMMatrixPerspectiveFovRH(mVerticalFOV, mAspect, mNearDepth, mFarDepth);

    XMStoreFloat4x4(&mProj, P);
}


void Camera::init( XMFLOAT3& t, float aspect, float fov, float n, float f )
{
    resetTranslation(t);
    updateLens(aspect, fov, n, f);
    updateView();
}

void Camera::resetTranslation( XMFLOAT3& t )
{
    mTranslation = t;
}

void Camera::updateLens( float aspect, float fov, float n, float f )
{
    mFrustum.setAll(aspect, fov, n, f);
}

void Camera::updateAspect( float aspect )
{
    mFrustum.setAspect(aspect);
}

void Camera::updateFOV( float fov )
{
    mFrustum.setAspect(fov);
}

void Camera::updateView()
{
    const XMFLOAT3* r = mRotation.getRightVector();
    const XMFLOAT3* u = mRotation.getUpVector();
    const XMFLOAT3* f = mRotation.getFrontVector();

    // Fill in the view matrix entries.
    XMVECTOR right       = XMLoadFloat3(r);
    XMVECTOR up          = XMLoadFloat3(u);
    XMVECTOR front       = XMLoadFloat3(f);
    XMVECTOR translation = XMLoadFloat3(&mTranslation);

    // Re-normalize
    front = XMVector3Normalize(front);
    up = XMVector3Normalize(XMVector3Cross(front, right));
    right = XMVector3Cross(up, front);

    float x = -XMVectorGetX(XMVector3Dot(translation, right));
    float y = -XMVectorGetX(XMVector3Dot(translation, up));
    float z = -XMVectorGetX(XMVector3Dot(translation, front));

    mView(0,0) = r->x;
    mView(1,0) = r->y; 
    mView(2,0) = r->z; 
    mView(3,0) = x;   

    mView(0,1) = u->x;
    mView(1,1) = u->y;
    mView(2,1) = u->z;
    mView(3,1) = y;  

    mView(0,2) = f->x; 
    mView(1,2) = f->y; 
    mView(2,2) = f->z; 
    mView(3,2) = z;   

    mView(0,3) = 0.0f;
    mView(1,3) = 0.0f;
    mView(2,3) = 0.0f;
    mView(3,3) = 1.0f;

    /*
    mView(0, 0) = r->x;
    mView(0, 1) = r->y; 
    mView(0, 2) = r->z; 
    mView(0, 3) = x;   

    mView(1,0) = u->x;
    mView(1,1) = u->y;
    mView(1,2) = u->z;
    mView(1,3) = y;  

    mView(2,0) = f->x; 
    mView(2,1) = f->y; 
    mView(2,2) = f->z; 
    mView(2,3) = z;   

    mView(3,0) = 0.0f;
    mView(3,1) = 0.0f;
    mView(3,2) = 0.0f;
    mView(3,3) = 1.0f;
    */
}
