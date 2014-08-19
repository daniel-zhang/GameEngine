#ifndef MATERIAL_ATTRIBUTE_H
#define MATERIAL_ATTRIBUTE_H

#include "reference.h"
#include "ShaderEnum.h"
#include "Shader.h"
/*
Attribute types:
    - HLSL built-in types
    - Engine defined types
*/
class MaterialAttributeInterface
{
public:
    MaterialAttributeInterface(EnumShaderVarTag inTag):mTag(inTag){}

    virtual void sync(ShaderParameter* sp) = 0;
    EnumShaderVarTag mTag;
};

//
// Template class for engine defined attribute types and most HLSL intrinsic types
// Restriction: T must have a valid default ctor
//
template <typename T>
class MaterialAttr : public MaterialAttributeInterface
{
public:
    MaterialAttr(EnumShaderVarTag inTag) : MaterialAttributeInterface(inTag) 
    {
        pData = new T;
    }
    ~MaterialAttr()
    {
        if (pData) delete pData;
    }
    virtual void sync(ShaderParameter* sp)
    {
        if (mTag == sp->mVarTag)
        {
            sp->mVar->SetRawValue(pData, 0, sizeof(T));
        }
    }
    T* pData;
};

//
// Specialization for some HLSL built-in types
//
template<>
class MaterialAttr<float> : public MaterialAttributeInterface
{
public:
    MaterialAttr(EnumShaderVarTag inTag) : MaterialAttributeInterface(inTag) 
    {
        pData = new float(0.f);
    }
    ~MaterialAttr<float>()
    {
        if (pData) delete pData;
    }
    virtual void sync(ShaderParameter* sp)
    {
        if (mTag == sp->mVarTag)
        {
            sp->mVar->AsScalar()->SetFloat(*pData);
        }
    }
    float* pData;
};

template<>
class MaterialAttr<XMFLOAT4> : public MaterialAttributeInterface
{
public:
    MaterialAttr(EnumShaderVarTag inTag):MaterialAttributeInterface(inTag)
    {
        pData = new XMFLOAT4(0.f, 0.f, 0.f, 0.f);
    }
    ~MaterialAttr<XMFLOAT4>()
    {
        if (pData) delete pData;
    }
    virtual void sync(ShaderParameter* sp)
    {
        if (mTag == sp->mVarTag)
        {
            sp->mVar->AsVector()->SetFloatVector(reinterpret_cast<const float*>( &(XMLoadFloat4(pData)) ) );
        }
    }
    XMFLOAT4* pData;
};

template<>
class MaterialAttr<XMFLOAT4X4>: public MaterialAttributeInterface
{
public:
    MaterialAttr(EnumShaderVarTag inTag):MaterialAttributeInterface(inTag)
    {
        pData = new XMFLOAT4X4;
        XMStoreFloat4x4(pData, XMMatrixIdentity());
    }
    ~MaterialAttr<XMFLOAT4X4>()
    {
        if(pData) delete pData;
    }
    virtual void sync(ShaderParameter* sp) 
    { 
        if (mTag == sp->mVarTag)
        {
            sp->mVar->AsMatrix()->SetMatrix(reinterpret_cast<const float*>( &(XMLoadFloat4x4(pData)) )); 
        }
    }
    XMFLOAT4X4* pData;
};

template<>
class MaterialAttr<ID3D11ShaderResourceView>:public MaterialAttributeInterface
{
public:
    MaterialAttr(EnumShaderVarTag inTag):MaterialAttributeInterface(inTag)
    {
        pData = NULL;
    }
    virtual void sync(ShaderParameter* sp)
    {
        if(mTag == sp->mVarTag)
            sp->mVar->AsShaderResource()->SetResource(pData);
    }
    ID3D11ShaderResourceView* pData;
};

//
// HLSL built-in types
//
/*
class FloatAttr : public MaterialAttributeBase
{
public:
    FloatAttr(EnumShaderVarTag inTag):MaterialAttributeBase(inTag){}
    virtual void sync(ShaderParameter* sp)
    {
        if (mTag == sp->mVarTag)
        {
            sp->mVar->AsScalar()->SetFloat(*pData);
        }
    }
    float* pData;
};

class FloatVector4Attr : public MaterialAttributeBase
{
public:
    FloatVector4Attr(EnumShaderVarTag inTag):MaterialAttributeBase(inTag){}
    virtual void sync(ShaderParameter* sp)
    {
        if (mTag == sp->mVarTag)
        {
            sp->mVar->AsVector()->SetFloatVector(reinterpret_cast<const float*>( &(XMLoadFloat4(pData)) ) );
        }
    }
    XMFLOAT4* pData;
};

class FloatMatrix4x4Attr : public MaterialAttributeBase
{
public:
    FloatMatrix4x4Attr(EnumShaderVarTag inTag):MaterialAttributeBase(inTag){}
    virtual void sync(ShaderParameter* sp) 
    { 
        if (mTag == sp->mVarTag)
        {
            sp->mVar->AsMatrix()->SetMatrix(reinterpret_cast<const float*>(&(XMLoadFloat4x4(pData)))); 
        }
    }
    XMFLOAT4X4* pData;
};
*/

//
// Engine defined types
//

// For example
#endif