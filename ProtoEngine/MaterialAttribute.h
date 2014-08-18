#ifndef MATERIAL_ATTRIBUTE_H
#define MATERIAL_ATTRIBUTE_H

#include "d3dx11.h"
#include "xnamath.h"
#include "d3dx11effect.h"
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
//
template <typename T>
class MaterialAttr : public MaterialAttributeInterface
{
public:
    MaterialAttr(EnumShaderVarTag inTag) : MaterialAttributeInterface(inTag) {}
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
    MaterialAttr(EnumShaderVarTag inTag) : MaterialAttributeInterface(inTag) {}
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
    MaterialAttr(EnumShaderVarTag inTag):MaterialAttributeInterface(inTag){}
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
    MaterialAttr(EnumShaderVarTag inTag):MaterialAttributeInterface(inTag){}
    virtual void sync(ShaderParameter* sp) 
    { 
        if (mTag == sp->mVarTag)
        {
            sp->mVar->AsMatrix()->SetMatrix(reinterpret_cast<const float*>( &(XMLoadFloat4x4(pData)) )); 
        }
    }
    XMFLOAT4X4* pData;
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