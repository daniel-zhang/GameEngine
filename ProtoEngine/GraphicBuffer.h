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

class IndexBuffer 
{
public:
    typedef std::vector<uint32> TLocalCache;
    IndexBuffer();
    ~IndexBuffer();

    void setLocalCache(TLocalCache& inIndices);
    bool createStaticGpuDataFromCache(RenderInterface* ri);
    void bind(RenderInterface* ri);
    // Not allow dynamic index buffer update for now
    bool createDynmGpuDataFromCache(RenderInterface* ri) { return false; }
    bool updateDynmGpuDataFromCache(RenderInterface* ri){return false;}
    void clear();

    TLocalCache& getLoalCache(){ return mLocalCache; }
    uint32 getNumIndex(){return mLocalCache.size();}
    uint32 getNumFace(){return mLocalCache.size()/3;}

protected:
    TLocalCache mLocalCache;
    ID3D11Buffer* mBuffer;
    bool mIsDynamic;
};

// Vertex format enum
enum EVertexFormat
{
    e_pos_normal_tex = 0,
    e_pos_normal_tan_tex,
    //e_vf_skinned,
};

// InputLayout description, specialized for each vertex format enum
template <EVertexFormat N> class InputLayoutDesc
{
public:
    static const uint32 desc_num;
    static const D3D11_INPUT_ELEMENT_DESC* format_desc;
};

// VertexFactory that defines vertex data structure, specialized for each vertex format enum
template <EVertexFormat N> class VertexFactory {};
template <> class VertexFactory<e_pos_normal_tex>
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
template <> class VertexFactory<e_pos_normal_tan_tex>
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
    typedef VertexFactory<N> VERTEX_TYPE;
    typedef std::vector<VERTEX_TYPE> TLocalCache;

    VertexBuffer():mVertexSize(sizeof(VERTEX_TYPE))
    {
        mNumGpuVert = 0;
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
        mLocalCache.clear();
        safe_release(&mBuffer);
        mNumGpuVert = 0;
    }
    void setLocalCache(TLocalCache& inVertices)
    {
        mLocalCache.resize(inVertices.size());
        std::copy(inVertices.begin(), inVertices.end(), mLocalCache.begin());
    }
    TLocalCache& getLocalCache()
    {
        return mLocalCache;
    }
    bool createStaticGpuDataFromCache(RenderInterface* ri)
    {
        if(mLocalCache.empty()) return false;
        mIsDynamic = false;

        D3D11_BUFFER_DESC vbd;
        vbd.Usage = D3D11_USAGE_IMMUTABLE;
        vbd.ByteWidth = mVertexSize * mLocalCache.size();
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = 0;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA vinitData;
        vinitData.pSysMem = &mLocalCache[0];

        d3d_check(ri->mDevice->CreateBuffer(&vbd, &vinitData, &mBuffer));
        return true;
    }
    bool createDynmGpuDataFromCache(RenderInterface* ri)
    {
        if (mLocalCache.empty()) return false;
        mIsDynamic = true;

        D3D11_BUFFER_DESC vbd;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.ByteWidth = mVertexSize * mLocalCache.size();
        vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = 0;
        vbd.Usage = D3D11_USAGE_DYNAMIC; 

        d3d_check(ri->mDevice->CreateBuffer(&vbd, 0, &mBuffer));
        return updateDynmGpuDataFromCache(ri);
    }
    bool updateDynmGpuDataFromCache(RenderInterface* ri)
    {
        if (mLocalCache.empty() || mIsDynamic == false) return false;

        D3D11_MAPPED_SUBRESOURCE mappedData;
        ri->mCtx->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

        VERTEX_TYPE* vStart = reinterpret_cast<VERTEX_TYPE*>(mappedData.pData);
        uint32 safeSize = mLocalCache.size() < mNumGpuVert ? mLocalCache.size() : mNumGpuVert;
        //std::copy(mLocalCache.begin(), mLocalCache.end(), &vStart[0]);
        memcpy(&vStart[0], &mLocalCache[0], sizeof(VERTEX_TYPE) * safeSize);

        ri->mCtx->Unmap(mBuffer, 0);
        return true;
    }
    void bind(RenderInterface* ri)
    {
        ri->mCtx->IASetVertexBuffers(0, 1, &mBuffer, &mVertexSize, &mVertexOffset);
        /*
        ri->mCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        ri->mCtx->IASetInputLayout(mInputLayout);
        */
    }
    /*
    bool createStatic(RenderInterface* ri, VERTEX_TYPE* pData, uint32 numVertices )
    {
        if(numVertices == 0 || pData == NULL) return false;
        mNumGpuVert = numVertices;
        mIsDynamic = false;

        D3D11_BUFFER_DESC vbd;
        vbd.Usage = D3D11_USAGE_IMMUTABLE;
        vbd.ByteWidth = mVertexSize * numVertices;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = 0;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA vinitData;
        vinitData.pSysMem = pData;

        d3d_check(ri->mDevice->CreateBuffer(&vbd, &vinitData, &mBuffer));
        return true;
    }
    bool createDynamic(RenderInterface* ri, uint32 numVertices)
    {
        if (numVertices == 0) return false;
        mNumGpuVert = numVertices;
        mIsDynamic = true;

        D3D11_BUFFER_DESC vbd;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.ByteWidth = mVertexSize * numVertices;
        vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vbd.MiscFlags = 0;
        vbd.StructureByteStride = 0;
        vbd.Usage = D3D11_USAGE_DYNAMIC; 

        d3d_check(ri->mDevice->CreateBuffer(&vbd, 0, &mBuffer));
        return true;
    }
    bool updateDynamic(RenderInterface* ri, VERTEX_TYPE* inputVertices, uint32 inputNum)
    {
        if(!mIsDynamic) return false;
        // Treat as failure when number of input vertices exceeds buffer size ?
        // if(inputNum > mNumVertices) return false;

        D3D11_MAPPED_SUBRESOURCE mappedData;
        ri->mCtx->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
        VERTEX_TYPE* vStart = reinterpret_cast<VERTEX_TYPE*>(mappedData.pData);
        for (UINT i = 0; i < inputNum; ++i)
        {
            vStart[i] = inputVertices[i];
        }
        ri->mCtx->Unmap(mBuffer, 0);

        return true;
    }
    */

protected:
    const uint32 mVertexSize;
    uint32 mNumGpuVert;
    uint32 mVertexOffset;

    ID3D11Buffer* mBuffer;
    TLocalCache mLocalCache;

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