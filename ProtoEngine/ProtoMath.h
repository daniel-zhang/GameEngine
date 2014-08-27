#ifndef PROTO_MATH_H
#define PROTO_MATH_H

#include <Windows.h>
#include "xnamath.h"

class ProtoMath
{
public:
    // Returns random float in [0, 1).
    static float RandF()
    {
        return (float)(rand()) / (float)RAND_MAX;
    }

    // Returns random float in [a, b).
    static float RandF(float a, float b)
    {
        return a + RandF()*(b-a);
    }

    template<typename T>
    static T Min(const T& a, const T& b)
    {
        return a < b ? a : b;
    }

    template<typename T>
    static T Max(const T& a, const T& b)
    {
        return a > b ? a : b;
    }

    template<typename T>
    static T Lerp(const T& a, const T& b, float t)
    {
        return a + (b-a)*t;
    }

    template<typename T>
    static T Clamp(const T& x, const T& low, const T& high)
    {
        return x < low ? low : (x > high ? high : x); 
    }

    // Returns the polar angle of the point (x,y) in [0, 2*PI).
    static float AngleFromXY(float x, float y);

    static XMMATRIX InverseTranspose(CXMMATRIX M)
    {
        // Inverse-transpose is just applied to normals.  So zero out 
        // translation row so that it doesn't get into our inverse-transpose
        // calculation--we don't want the inverse-transpose of the translation.
        XMMATRIX A = M;
        A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

        XMVECTOR det = XMMatrixDeterminant(A);
        return XMMatrixTranspose(XMMatrixInverse(&det, A));
    }

    static XMVECTOR RandUnitVec3();
    static XMVECTOR RandHemisphereUnitVec3(XMVECTOR n);

    static const float Infinity;
    static const float Pi;
};

enum ECoordSystem
{
    e_LH_YUp_XRight = 0, // d3d default
    e_LH_ZUp_YRight, // unreal engine 3
    e_RH_ZUp_NYRight, // 3d max?
};

void SetCoordinateSystem(ECoordSystem inCoordSys, XMFLOAT3& outUp, XMFLOAT3& outRight, XMFLOAT3& outFront);
bool IsLeftHanded(ECoordSystem e);

// TODO: Evolve from Euler Angle to Quaternion
class Rotator
{
public:
    Rotator(bool forceOrtho = true, ECoordSystem coordSys = e_LH_YUp_XRight);
    void reset();
    void reset(ECoordSystem eCoordSys);

    // Looking up and down
    void pitch(float angle);
    // Rotate around Up axis
    void yaw(float angle);
    // Rotate around Front axis
    void roll(float angle);

    const XMFLOAT3* getUpVector() { return &mUp; }
    const XMFLOAT3* getRightVector() { return &mRight; }
    const XMFLOAT3* getFrontVector() { return &mFront; }

protected:
    void makeOrthogonal();

protected:
    ECoordSystem mCoordSys;
    bool mForceOrtho;
    XMFLOAT3 mUp, mRight, mFront;
};

#endif