#ifndef ATTRIBUTE_REFERENCE_H
#define ATTRIBUTE_REFERENCE_H

#include "Shader.h"
#include "ShaderEnum.h"

//
// An abstract base class to enable all TAttrRef<...> instances to be aggregated in a container
//
class AttrReferenceInterface
{
public:
    virtual NativeEnum_ShaderVarTag tag_enum() = 0;
    virtual void assign_to(ShaderParameter* sp) = 0;
};

//
// Generic TData types
//
template <typename TData, NativeEnum_ShaderVarTag TAG>
class TAttrRef : public AttrReferenceInterface
{
public:
    TAttrRef(TData& input) : mTag(TAG) { mReference = &input; }
    virtual NativeEnum_ShaderVarTag tag_enum() { return mTag; }
    virtual void assign_to(ShaderParameter* sp)
    {
        if (mTag == sp->mTag) sp->mVar->SetRawValue(mReference, 0, sizeof(TData));
    }
protected:
    TData* mReference;
    const NativeEnum_ShaderVarTag mTag;
};

//
// Partial Specialization on TData: TData*  
// (TData** is too fancy, ignored)
//
template <typename TData, NativeEnum_ShaderVarTag TAG>
class TAttrRef<TData*, TAG> : public AttrReferenceInterface
{
public:
    TAttrRef(TData* input) : mTag(TAG) { mReference = &input; }
    virtual NativeEnum_ShaderVarTag tag_enum() { return mTag; }
    virtual void assign_to(ShaderParameter* sp)
    {
        if (mTag == sp->mTag) sp->mVar->SetRawValue(*mReference, 0, sizeof(TData));
    }
protected:
    TData** mReference;
    const NativeEnum_ShaderVarTag mTag;
};

//
// Partial Specialization on TData: float
//
template<NativeEnum_ShaderVarTag TAG>
class TAttrRef<float, TAG> : public AttrReferenceInterface
{
public:
    TAttrRef(float& input) : mTag(TAG) { mReference = &input; }
    virtual NativeEnum_ShaderVarTag tag_enum() { return mTag; }
    virtual void assign_to(ShaderParameter* sp)
    {
        if (mTag == sp->mTag) sp->mVar->AsScalar()->SetFloat(*mReference);
    }
protected:
    float* mReference;
    const NativeEnum_ShaderVarTag mTag;

};
//
// Partial Specialization on TData: XMFLOAT4
//
template<NativeEnum_ShaderVarTag TAG>
class TAttrRef<XMFLOAT4, TAG> : public AttrReferenceInterface
{
public:
    TAttrRef(XMFLOAT4& input) : mTag(TAG) { mReference = &input; }
    virtual NativeEnum_ShaderVarTag tag_enum() { return mTag; }
    virtual void assign_to(ShaderParameter* sp)
    {
        if (mTag == sp->mTag) sp->mVar->AsVector()->SetFloatVector( reinterpret_cast<const float*>( &( XMLoadFloat4(mReference) ) ) );
    }
protected:
    XMFLOAT4* mReference;
    const NativeEnum_ShaderVarTag mTag;
};
//
// Partial Specialization on TData: XMFLOAT4X4
//
template<NativeEnum_ShaderVarTag TAG>
class TAttrRef<XMFLOAT4X4, TAG> : public AttrReferenceInterface
{
public:
    TAttrRef(XMFLOAT4X4& input) : mTag(TAG) { mReference = &input; }
    virtual NativeEnum_ShaderVarTag tag_enum() { return mTag; }
    virtual void assign_to(ShaderParameter* sp)
    {
        if (mTag == sp->mTag) sp->mVar->AsMatrix()->SetMatrix( reinterpret_cast<const float*>( &( XMLoadFloat4x4(mReference) ) ) );
    }
protected:
    XMFLOAT4X4* mReference;
    const NativeEnum_ShaderVarTag mTag;
};
//
// Partial Specialization on TData: ID3D11ShaderResourceView
//
template<NativeEnum_ShaderVarTag TAG>
class TAttrRef<ID3D11ShaderResourceView*, TAG> : public AttrReferenceInterface
{
public:
    TAttrRef(ID3D11ShaderResourceView* input) : mTag(TAG) { mReference = &input; }
    virtual NativeEnum_ShaderVarTag tag_enum() { return mTag; }
    virtual void assign_to(ShaderParameter* sp)
    {
        if (mTag == sp->mTag) sp->mVar->AsShaderResource()->SetResource(*mReference);
    }
protected:
    ID3D11ShaderResourceView** mReference;
    const NativeEnum_ShaderVarTag mTag;
};

#endif