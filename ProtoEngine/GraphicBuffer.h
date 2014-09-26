#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <algorithm>
#include "reference.h"
#include "RenderInterface.h"
//#include "VertexFormat.h"

// Static assertion
template <bool b> struct StaticAssert {};
template <> struct StaticAssert<true>
{
    static void validate() {}
};

class RenderInterface;

// Deprecated
class GraphicBuffer
{
public:
    virtual void setLocalCache(RenderInterface*);
    virtual void getLocalCache(RenderInterface*);
    virtual void createStaticGpuData(RenderInterface*);
    virtual void createDynmGpuData(RenderInterface*);

    virtual void bind();

    virtual void updateGpuData(RenderInterface*);
    virtual void releaseGpuData();
};

/*
IndexBuffer and VertexBuffer share the same design:
Both of them manage render pipeline buffers and hold a system memory copy of their data
VertexBuffer is a class template, as IndexBuffer always holds an array of uint32, while vertex buffer content has to be specialized on EVertexFormat
*/
class IndexBuffer 
{
public:
    typedef std::vector<uint32> TIndexArray;
    IndexBuffer();
    ~IndexBuffer();

    // Style one: set index one by one
    void setIndArrayCapacity(uint32 capacity);
    void setIndexAt(uint32 ind, uint32 at);

    // Style two: set the whole array
    void setIndArray(TIndexArray& inIndices);
    TIndexArray& getIndArray(){ return mIndices; }
    void clearIndArray();

    bool createStaticGpuBuffer(RenderInterface* ri);
    // Not allow for dynamic index buffer at the moment
    bool createDynamicGpuBuffer(RenderInterface* ri) { return false; }
    bool updateGpuBuffer(RenderInterface* ri){return false;}
    void bindGpuBuffer(RenderInterface* ri);

    uint32 numVertIndices(){return mIndices.size();}
    uint32 numTriangles(){return mIndices.size()/3;}

protected:
    TIndexArray mIndices;
    ID3D11Buffer* mBuffer;
    bool mIsDynamic;
};

/* Vertex format enumerations
The enumerations are then used by a subsequent bunch of template classes as template argument.
This approach to support multiple vertex formats is borrowed from UE2.5

Another implementation is to define separated attribute collections(locations, normals, UVs, tangents, etc)
and provide a set of methods to set and combine them during mesh construction.

The approach used here is less flexible compared to the mentioned one.
*/
enum EVertexFormat
{
    e_pos_normal_tex = 0,
    e_pos_normal_tan_tex,
};

/* InputLayout description, specialized for each vertex format enum
An InputLayout template class is always corresponded to a specific VertexFactory template class
Notice that the template specialization is implemented in the .cpp file, via static variable assignment.
*/
template <EVertexFormat N> 
class InputLayoutDesc
{
public:
    static const uint32 desc_num;
    static const D3D11_INPUT_ELEMENT_DESC* format_desc;
};

// An empty template that is to be specialized to define each vertex data structure
template <EVertexFormat N> 
class VertexFactory {};

// VertexFactory specialized for: e_pos_normal_tex
template <> 
class VertexFactory<e_pos_normal_tex>
{
public:
    VertexFactory<e_pos_normal_tex>(){ZeroMemory(this, sizeof(VertexFactory<e_pos_normal_tex>));}
    VertexFactory<e_pos_normal_tex>(
        float px, float py, float pz, 
        float nx, float ny, float nz,
        float u, float v) 
        :
        position(px,py,pz), normal(nx,ny,nz), texcoord(u,v){}
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texcoord;
};

// VertexFactory specialized for: e_pos_normal_tan_tex
template <> 
class VertexFactory<e_pos_normal_tan_tex>
{
public:
    VertexFactory<e_pos_normal_tan_tex>(){ZeroMemory(this, sizeof(VertexFactory<e_pos_normal_tex>));}
    VertexFactory<e_pos_normal_tan_tex>(
        float px, float py, float pz, 
        float nx, float ny, float nz,
        float tx, float ty, float tz,
        float u, float v) 
        : 
        position(px,py,pz), normal(nx,ny,nz), tangent(tx, ty, tz), texcoord(u,v){}

    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT3 tangent;
    XMFLOAT2 texcoord;
};

//
// Combines layout desc and format data structure into a meaningful resource. Specialized on vertex format enum
//
template<EVertexFormat N>
class VertexBuffer
{
public:
    typedef VertexFactory<N> TVertexType;
    typedef std::vector<TVertexType> TVertexArray;

    VertexBuffer()
    {
        mVertexOffset = 0;
        mIsDynamic = false;
        mBuffer = NULL;
    }
    ~VertexBuffer()
    {
        clear();
    }

    void clear()
    {
        mVertexOffset = 0;
        mIsDynamic = false;
        mVertArray.clear();
        safe_release(&mBuffer);
    }
    bool isDynamic() { return mIsDynamic; }
    uint32 stride() { return sizeof(TVertexType); }
    uint32 numVerts() { return mVertArray.size(); }

    // Style one: set vert at the specific location
    void setVertArrayCapacity(uint32 capacity)
    {
        mVertArray.resize(capacity);
    }
    void setVertAt(TVertexType& vert, uint32 at)
    {
        mVertArray[at] = vert;
    }

    // Style two: set the whole vert array 
    void setVertArray(TVertexArray& inVertices)
    {
        mVertArray.resize(inVertices.size());
        std::copy(inVertices.begin(), inVertices.end(), mVertArray.begin());
    }
    TVertexArray& getVertArray() 
    { 
        return mVertArray; 
    }

    bool createStaticGpuBuffer(RenderInterface* ri)
    {
        if(mVertArray.empty()) return false;
        mIsDynamic = false;

        D3D11_BUFFER_DESC vbd;
        vbd.Usage = D3D11_USAGE_IMMUTABLE;
        vbd.ByteWidth = stride() * numVerts();
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = 0;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA vinitData;
        vinitData.pSysMem = &mVertArray[0];

        d3d_check(ri->mDevice->CreateBuffer(&vbd, &vinitData, &mBuffer));
        return true;
    }
    bool createDynamicGpuBuffer(RenderInterface* ri)
    {
        if (mVertArray.empty()) return false;
        mIsDynamic = true;

        D3D11_BUFFER_DESC vbd;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.ByteWidth = stride() * numVerts();
        vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = 0;
        vbd.Usage = D3D11_USAGE_DYNAMIC; 

        d3d_check(ri->mDevice->CreateBuffer(&vbd, 0, &mBuffer));
        return true; 
    }
    void updateGpuBuffer(RenderInterface* ri)
    {
        D3D11_MAPPED_SUBRESOURCE mappedData;
        ri->mCtx->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

        TVertexType* vStart = reinterpret_cast<TVertexType*>(mappedData.pData);
        //uint32 safeSize = mVertArray.size() < mNumGpuVert ? mVertArray.size() : mNumGpuVert;
        //std::copy(mLocalCache.begin(), mLocalCache.end(), &vStart[0]);
        memcpy(&vStart[0], &mVertArray[0], sizeof(TVertexType) * mVertArray.size());

        ri->mCtx->Unmap(mBuffer, 0);
    }
    void bindGpuBuffer(RenderInterface* ri)
    {
        uint32 bufferStride = stride();
        ri->mCtx->IASetVertexBuffers(0, 1, &mBuffer, &bufferStride, &mVertexOffset);
        /*
        ri->mCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        ri->mCtx->IASetInputLayout(mInputLayout);
        */
    }
 
protected:
    uint32 mVertexOffset;

    ID3D11Buffer* mBuffer;
    TVertexArray mVertArray;

    bool mIsDynamic;
};

void graphic_buffer_test();

/////////////////////////////////////////////////////////////////////////////////////
// THE FOLLOWING STUFF IS NOT ENGINE CODE, JUST TO TEST TEMPLATE SPECIALIZATION!
/////////////////////////////////////////////////////////////////////////////////////
// partial specialization
enum ESemantic
{
    e_shader_var_0 = 0,
    e_shader_var_1, 
    e_shader_var_2,
};

/*
float
xmfloat4
xmfloat4x4             world_matrix
ShaderResourceView     texture
MeshMaterial           material
*/
class SimParam
{
public:
    enum EMyType
    {
        e_raw = 0,
        e_float,
        e_int,
        e_bool
    };
    void procRawMem(void* data, int offset, int count){mType = e_raw;}
    void procFloat(float d){mType = e_float;}
    void procInt(int d){mType = e_int;}
    void procBool(bool d){mType = e_bool;}

    EMyType mType;
};

class AutoAttrInterface
{
public:
    virtual void sync_to(SimParam* target) = 0;
    virtual ESemantic getSemantic() = 0;
};

#include <map>
class SimParamCollection
{
public:
    std::map<ESemantic, SimParam> mSemanticParamMap;
    typedef std::map<ESemantic, SimParam>::iterator TMapIter ;

    void set(AutoAttrInterface* aai)
    {
        TMapIter iter = mSemanticParamMap.find(aai->getSemantic());
        if (iter != mSemanticParamMap.end())
        {
            aai->sync_to(&(iter->second));
        }
    }
};

template<typename DataType, ESemantic sem>
class TVarRef : public AutoAttrInterface
{
public:
    TVarRef():mSem(sem){}
    void bind(DataType& data){mRef = &data;}
    void sync_to(SimParam* target) { target->procRawMem(mRef, 0, sizeof(DataType)); }
    ESemantic getSemantic(){return mSem;}

    ESemantic mSem;
    DataType* mRef;
};
template<typename DataType, ESemantic sem>
class TVarRef<DataType*, sem> : public AutoAttrInterface
{
public:

};

template<ESemantic sem>
class TVarRef<bool, sem> : public AutoAttrInterface
{
public:
    TVarRef():mSem(sem)
    {
        mRef = NULL;
    }
    void bind(bool& data){mRef = &data;}
    void sync_to(SimParam* target) { target->procBool(*mRef); }
    ESemantic getSemantic(){return mSem;}
    bool* mRef;
    const ESemantic mSem;
};

template<ESemantic sem>
class TVarRef<int, sem> : public AutoAttrInterface
{
public:
    TVarRef():mSem(sem){}
    void bind(int& data){mRef = &data;}
    void sync_to(SimParam* target) { target->procInt(*mRef); }
    ESemantic getSemantic(){return mSem;}
    int* mRef;
    const ESemantic mSem;
};


#endif