#include "GraphicBuffer.h"

IndexBuffer::IndexBuffer()
{
    mBuffer = NULL;
    mIsDynamic = false;
}

IndexBuffer::~IndexBuffer()
{
    clear();
}

void IndexBuffer::setLocalCache( TLocalCache& inIndices )
{
    uint32 numIndices = inIndices.size();
    mLocalCache.reserve(numIndices);
    std::copy(inIndices.begin(), inIndices.end(), mLocalCache.begin());
}

bool IndexBuffer::createStaticGpuDataFromCache( RenderInterface* ri )
{
    if (mLocalCache.empty()) return false;

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * getNumIndex();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &mLocalCache[0];

    d3d_check(ri->mDevice->CreateBuffer(&ibd, &iinitData, &mBuffer));
    return true;
}

void IndexBuffer::bind( RenderInterface* ri )
{
    ri->mCtx->IASetIndexBuffer(mBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer::clear()
{
    mLocalCache.clear();
    safe_release(&mBuffer);
    mIsDynamic = false;

}



/*
template<> D3D11_INPUT_ELEMENT_DESC InputLayoutDesc<e_pos_normal_tex>::format_desc[3] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0} 
};
*/

static const D3D11_INPUT_ELEMENT_DESC DESC_POS_NORMAL_TEX[3] = {
//SemanticName; SemanticIndex; Format; InputSlot; AlignedByteOffset; InputSlotClass; InstanceDataStepRate;
    {"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,    0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0} 
};
static const D3D11_INPUT_ELEMENT_DESC DESC_POS_NORMAL_TANGENT_TEX[4] = {
    {"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,    0, 36,  D3D11_INPUT_PER_VERTEX_DATA, 0} 
};
template<> const uint32 InputLayoutDesc<e_pos_normal_tex>::desc_num = 3; 
template<> const D3D11_INPUT_ELEMENT_DESC* InputLayoutDesc<e_pos_normal_tex>::format_desc = DESC_POS_NORMAL_TEX; 
template<> const uint32 InputLayoutDesc<e_pos_normal_tan_tex>::desc_num = 4; 
template<> const D3D11_INPUT_ELEMENT_DESC* InputLayoutDesc<e_pos_normal_tan_tex>::format_desc = DESC_POS_NORMAL_TANGENT_TEX; 

void graphic_buffer_test()
{
    StaticAssert<sizeof(VertexFactory<e_pos_normal_tex>) == sizeof(VertexBuffer<e_pos_normal_tex>::VERTEX_TYPE) >::validate();
    StaticAssert<sizeof(VertexFactory<e_pos_normal_tan_tex>) == sizeof(VertexBuffer<e_pos_normal_tan_tex>::VERTEX_TYPE) >::validate();
    StaticAssert<InputLayoutDesc<e_pos_normal_tex>::desc_num == 3>::validate();
    StaticAssert<InputLayoutDesc<e_pos_normal_tan_tex>::desc_num == 4>::validate();

    VertexBuffer<e_pos_normal_tex> testBuffer;
}

