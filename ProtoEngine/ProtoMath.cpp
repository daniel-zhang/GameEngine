#include "ProtoMath.h"
#include <float.h>
#include <cmath>


const float ProtoMath::Infinity = FLT_MAX;
const float ProtoMath::Pi       = 3.1415926535f;

float ProtoMath::AngleFromXY(float x, float y)
{
    float theta = 0.0f;

    // Quadrant I or IV
    if(x >= 0.0f) 
    {
        // If x = 0, then atanf(y/x) = +pi/2 if y > 0
        //                atanf(y/x) = -pi/2 if y < 0
        theta = atanf(y / x); // in [-pi/2, +pi/2]

        if(theta < 0.0f)
            theta += 2.0f*Pi; // in [0, 2*pi).
    }

    // Quadrant II or III
    else      
        theta = atanf(y/x) + Pi; // in [0, 2*pi).

    return theta;
}

XMVECTOR ProtoMath::RandUnitVec3()
{
    XMVECTOR One  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
    XMVECTOR Zero = XMVectorZero();

    // Keep trying until we get a point on/in the hemisphere.
    while(true)
    {
        // Generate random point in the cube [-1,1]^3.
        XMVECTOR v = XMVectorSet(ProtoMath::RandF(-1.0f, 1.0f), ProtoMath::RandF(-1.0f, 1.0f), ProtoMath::RandF(-1.0f, 1.0f), 0.0f);

        // Ignore points outside the unit sphere in order to get an even distribution 
        // over the unit sphere.  Otherwise points will clump more on the sphere near 
        // the corners of the cube.

        if( XMVector3Greater( XMVector3LengthSq(v), One) )
            continue;

        return XMVector3Normalize(v);
    }
}

XMVECTOR ProtoMath::RandHemisphereUnitVec3(XMVECTOR n)
{
    XMVECTOR One  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
    XMVECTOR Zero = XMVectorZero();

    // Keep trying until we get a point on/in the hemisphere.
    while(true)
    {
        // Generate random point in the cube [-1,1]^3.
        XMVECTOR v = XMVectorSet(ProtoMath::RandF(-1.0f, 1.0f), ProtoMath::RandF(-1.0f, 1.0f), ProtoMath::RandF(-1.0f, 1.0f), 0.0f);

        // Ignore points outside the unit sphere in order to get an even distribution 
        // over the unit sphere.  Otherwise points will clump more on the sphere near 
        // the corners of the cube.

        if( XMVector3Greater( XMVector3LengthSq(v), One) )
            continue;

        // Ignore points in the bottom hemisphere.
        if( XMVector3Less( XMVector3Dot(n, v), Zero ) )
            continue;

        return XMVector3Normalize(v);
    }
}

void SetCoordinateSystem( ECoordSystem inCoordSys, XMFLOAT3& outUp, XMFLOAT3& outRight, XMFLOAT3& outFront )
{
    XMFLOAT3 X(1.f, 0.f, 0.f);
    XMFLOAT3 Y(0.f, 1.f, 0.f);
    XMFLOAT3 Z(0.f, 0.f, 1.f);
    switch(inCoordSys)
    {
    case e_LH_YUp_XRight:
        outUp    = Y;
        outRight = X;
        outFront = Z;
        break;
    case e_LH_ZUp_YRight:
        outUp    = Z;
        outRight = Y;
        outFront = X;
        break;

    default:
        break;
    }
}

bool IsLeftHanded( ECoordSystem e )
{
    switch(e)
    {
    case e_LH_ZUp_YRight:
    case e_LH_YUp_XRight:
        return true;

    case e_RH_ZUp_NYRight:
    default:
        return false;
    }
}

Rotator::Rotator( bool forceOrtho /*= true*/, ECoordSystem coordSys /*= e_LH_YUp_XRight*/ ) :mForceOrtho(forceOrtho), mCoordSys(coordSys)
{
    SetCoordinateSystem(mCoordSys, mUp, mRight, mFront);
}

void Rotator::reset()
{
    SetCoordinateSystem(mCoordSys, mUp, mRight, mFront);
}

void Rotator::reset( ECoordSystem eCoordSys )
{
    mCoordSys = eCoordSys;
    SetCoordinateSystem(mCoordSys, mUp, mRight, mFront);
}

void Rotator::pitch( float angle )
{
    XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

    XMStoreFloat3(&mUp,   XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
    XMStoreFloat3(&mFront, XMVector3TransformNormal(XMLoadFloat3(&mFront), R));

    makeOrthogonal();
}

void Rotator::yaw( float angle )
{
    XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mUp), angle);

    XMStoreFloat3(&mRight,   XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
    XMStoreFloat3(&mFront, XMVector3TransformNormal(XMLoadFloat3(&mFront), R));

    makeOrthogonal();
}

void Rotator::roll( float angle )
{
    XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mFront), angle);

    XMStoreFloat3(&mRight,   XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
    XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));

    makeOrthogonal();
}

void Rotator::makeOrthogonal()
{
    if (!mForceOrtho)
        return;

    XMVECTOR R = XMLoadFloat3(&mRight);
    XMVECTOR U = XMLoadFloat3(&mUp);
    XMVECTOR F = XMLoadFloat3(&mFront);

    // Keep camera's axes orthogonal to each other and of unit length.
    F = XMVector3Normalize(F);
    U = XMVector3Normalize(XMVector3Cross(F, R));
    // U, L already ortho-normal, so no need to normalize cross product.
    R = XMVector3Cross(U, F); 

    XMStoreFloat3(&mRight, R);
    XMStoreFloat3(&mUp, U);
    XMStoreFloat3(&mFront, F);
}
