#include "GraphicBuffer.h"

IndexBuffer::IndexBuffer()
{
    mBuffer = NULL;
    mIsDynamic = false;
}

IndexBuffer::~IndexBuffer()
{
    clearIndArray();
}

void IndexBuffer::setIndArray( TIndexArray& inIndices )
{
    uint32 numIndices = inIndices.size();
    mIndices.resize(numIndices);
    std::copy(inIndices.begin(), inIndices.end(), mIndices.begin());
}

bool IndexBuffer::createStaticGpuBuffer( RenderInterface* ri )
{
    if (mIndices.empty()) return false;

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * numVertIndices();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &mIndices[0];

    d3d_check(ri->mDevice->CreateBuffer(&ibd, &iinitData, &mBuffer));
    return true;
}

void IndexBuffer::bindGpuBuffer( RenderInterface* ri )
{
    ri->mCtx->IASetIndexBuffer(mBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer::clearIndArray()
{
    mIndices.clear();
    safe_release(&mBuffer);
    mIsDynamic = false;

}

void IndexBuffer::setIndArrayCapacity( uint32 capacity )
{
    mIndices.resize(capacity, 0);
}

void IndexBuffer::setIndexAt( uint32 ind, uint32 at )
{
    mIndices[at] = ind;
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

#include "Material.h"
void graphic_buffer_test()
{
    StaticAssert<sizeof(VertexFactory<e_pos_normal_tex>) == sizeof(VertexBuffer<e_pos_normal_tex>::TVertexType) >::validate();
    StaticAssert<sizeof(VertexFactory<e_pos_normal_tan_tex>) == sizeof(VertexBuffer<e_pos_normal_tan_tex>::TVertexType) >::validate();
    StaticAssert<InputLayoutDesc<e_pos_normal_tex>::desc_num == 3>::validate();
    StaticAssert<InputLayoutDesc<e_pos_normal_tan_tex>::desc_num == 4>::validate();

    VertexBuffer<e_pos_normal_tex> testBuffer;

    ////////////////////////////////////
    SimParam target;

    uint32 t0 = 100;
    int t1 = 10;
    bool t2 = true;

    // Datatype, semantic, name
    TVarRef<uint32, e_shader_var_0> var0;
    TVarRef<int, e_shader_var_1> var1;
    TVarRef<bool, e_shader_var_2> var2;

    var0.bind(t0);
    var1.bind(t1);
    var2.bind(t2);

    var0.sync_to(&target);
    var1.sync_to(&target);
    var2.sync_to(&target);

    //DefaultMaterial m(std::string(""));

}

