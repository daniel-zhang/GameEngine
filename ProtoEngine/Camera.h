#ifndef CAMERA_H
#define CAMERA_H

#include "ProtoMath.h"
#include "Entity.h"

class Frustum
{
public:
    Frustum(ECoordSystem e = e_LH_YUp_XRight);
    void setAll( float aspect, float vFOV, float nearDepth, float farDepth );
    void setAspect(float aspect);
    void setFOV(float vFOV);

    float getNearWindowHeight() { return mNearWindowHeight; }
    float getNearWindowWidth() { return mNearWindowWidth; }
    float getFarWindowHeight(){return mFarWindowHeight;}
    float getFarWindowWidth(){return mFarWindowWidth;}
    const XMFLOAT4X4& getProj(){return mProj;}
    XMFLOAT4X4 getProjCopy(){return mProj;}

protected:
    void sync();
    void syncNearFarWindowSize();
    void syncProj();

protected:
    float mNearDepth;
    float mFarDepth;
    float mAspect;
    float mVerticalFOV;
    ECoordSystem mCoordSys;

    // Cached data to save calculation
    XMFLOAT4X4 mProj;
    float mNearWindowHeight;
    float mNearWindowWidth;
    float mFarWindowHeight;
    float mFarWindowWidth;
};

class Camera : public IControllable
{
public:
    Camera(){}
    void init(XMFLOAT3& t, float aspect, float fov, float n, float f);
    void resetTranslation(XMFLOAT3& t);
    void updateLens(float aspect, float fov, float n, float f);
    void updateAspect(float aspect);
    void updateFOV(float fov);

    // Construct a view matrix from the current translation and rotation
    void updateView();

    const Frustum* getFrustum() { return &mFrustum; }
    const XMFLOAT4X4& getView() { return mView; }
    const XMFLOAT4X4& getProj() { return mFrustum.getProj(); }
    XMFLOAT4X4 getViewCopy() { return mView; }
    XMFLOAT4X4 getProjCopy() { return mFrustum.getProjCopy(); }

protected:
    Frustum mFrustum;
    XMFLOAT4X4 mView;
};

class DirLight
{
public:
    DirLight(){ZeroMemory(this, sizeof(this));}

    XMFLOAT4 ambient;
    XMFLOAT4 diffuse;
    XMFLOAT4 specular;
    XMFLOAT3 direction;
    float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

#endif