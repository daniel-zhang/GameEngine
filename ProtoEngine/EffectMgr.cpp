#include "EffectMgr.h"
#include "RenderInterface.h"
#include "GameResourcePath.h"
#include "D3DUtilities.h"
#include "OSInterface.h"

#include <D3Dcompiler.h>
#include <d3dx11.h>

#include "Material.h"

EffectMgr::EffectMgr()
{
    mDir = std::string(gShaderPath);
    mDir = gShaderPath;
    mExtension = "";
}

EffectMgr::~EffectMgr()
{

}

bool EffectMgr::init(RenderInterface* ri)
{
    if (ri->isInitialized() == false)
        return false;

    MemBlock rawMem;
    std::string path = gShaderPath + std::string("base.fxo");
    OSInterface::fileToMem(path, rawMem );

    ID3DX11Effect* fx= NULL;
    d3d_check( 
        D3DX11CreateEffectFromMemory(rawMem.mData, rawMem.mSize, 0, ri->mDevice, &fx)
        );
    ///////////////////////////////////////////////////////////////////////
    // Test begin

    /*
    EffectInspector ei(effect);
    ei.go();
    */

    MaterialAttr<XMFLOAT4X4> Local2World(local_to_world);

    XMMATRIX identityMatrix = XMMatrixIdentity();
    Local2World.pData = new XMFLOAT4X4();
    XMStoreFloat4x4(Local2World.pData, identityMatrix);

    MaterialAttr<XMFLOAT3> CamPos(cam_pos);
    CamPos.pData = new XMFLOAT3(1.f, 1.f, 1.f);

    std::vector<MaterialAttributeInterface*> tmp;
    tmp.push_back(&Local2World);
    tmp.push_back(&CamPos);

    Shader shader(fx, &mTagDefinition);
    shader.init();
    shader.syncFrom(tmp);

    delete CamPos.pData;
    delete Local2World.pData;

    // Test end
    ///////////////////////////////////////////////////////////////////////

    return true;
}

bool EffectMgr::destroy()
{
    return true;
}

Shader* EffectMgr::getByName( const char* effectName )
{

    return NULL;
}

Shader* EffectMgr::getByIndex()
{
    return NULL;

}
