#include "VertexFormatMgr.h"
#include "RenderInterface.h"
#include "Shader.h"
#include "EffectMgr.h"

const D3D11_INPUT_ELEMENT_DESC desc_pos_normal_tex[3] = 
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0} 
};

VertexFormat::
VertexFormat(RenderInterface* ri, Shader* inShader, EnumVertexFormat eVFormat, D3D11_PRIMITIVE_TOPOLOGY eTopology, const D3D11_INPUT_ELEMENT_DESC* pDesc, uint32 numDesc )
{
    mInputLayout = NULL;
    mEnumVF = eVFormat;
    mEnumTopology = eTopology;

    D3DX11_PASS_DESC passDesc;
    inShader->mFx->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetDesc(&passDesc);
    d3d_check( ri->mDevice->CreateInputLayout( 
        pDesc, numDesc, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout ) );
}

VertexFormat::~VertexFormat()
{
    safe_release(&mInputLayout);
}

void VertexFormat::attach( RenderInterface* ri )
{
    ri->mCtx->IASetInputLayout(mInputLayout);
    ri->mCtx->IASetPrimitiveTopology(mEnumTopology);
}


VertexFormatMgr::VertexFormatMgr() { } 
VertexFormatMgr::~VertexFormatMgr() { }

bool VertexFormatMgr::init( RenderInterface* ri )
{
    mVFormats.push_back(VertexFormat( 
        ri,
        //TODO
        Singleton<EffectMgr>::getInstance().getByName("some_effect"),
        VF_POS_NORMAL_TEX,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        desc_pos_normal_tex, 3 
    )); 

    return true;
}

bool VertexFormatMgr::destroy()
{
    mVFormats.clear();

    return true;
}

VertexFormat* VertexFormatMgr::get( EnumVertexFormat evf )
{
    return &mVFormats[evf];
}
