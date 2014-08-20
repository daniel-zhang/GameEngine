#include "EffectMgr.h"
#include "RenderInterface.h"
#include "Material.h"

#include "OSInterface.h"
#include "GameResourcePath.h"

#include <D3Dcompiler.h>
#include <d3dx11.h>


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

//
//
//

// TODO 
// shader names should be read from config or scanned from shader dir
const char* gShaderNames[] = {
    "base"
};

ShaderMgr::ShaderMgr()
{

}

ShaderMgr::~ShaderMgr()
{
}

bool ShaderMgr::init( RenderInterface* ri )
{
    buildShaderNameMap();
    // Load shaders
    for (uint32 i = 0; i < mShaderNames.size(); ++i)
    {
        std::string fullname = gShaderPath + mShaderNames[i] + ".fxo";
        MemBlock rawMem;
        if ( OSInterface::fileToMem(fullname, rawMem ) == false )
        {
            // throw error 
        }
        ID3DX11Effect* fx= NULL;
        d3d_check( D3DX11CreateEffectFromMemory(rawMem.mData, rawMem.mSize, 0, ri->mDevice, &fx) );

        Shader* shader = new Shader(fx, &mTagDefinition);
        mShaders.push_back(shader);
    }

    return true;
}

bool ShaderMgr::destroy()
{
    for (uint32 i = 0; i < mShaders.size(); ++i)
    {
        if (mShaders[i])
            delete mShaders[i];
    }
    mShaders.clear();
    mShaderNames.clear();
    mShaderNameMap.clear();

    return true;
}

Shader* ShaderMgr::getShaderByName( std::string& shaderName )
{
    uint32 shaderIndex = 0;
    if (shaderNameToIndex(shaderName, shaderIndex) == false)
        return NULL;

    return mShaders[shaderIndex];
}

void ShaderMgr::buildShaderNameMap()
{
    mShaderNameMap.clear();
    uint32 numShader = ARRAYSIZE(gShaderNames);
    for (uint32 i = 0; i < numShader; ++i)
    {
        mShaderNames.push_back(std::string(gShaderNames[i]));
        mShaderNameMap.insert(std::pair<std::string, uint32>(std::string(gShaderNames[i]), i));
    }
}

bool ShaderMgr::shaderNameToIndex( const std::string& inShaderName, uint32& outIndex)
{
    ShaderNameMap::const_iterator citor = mShaderNameMap.find(inShaderName);
    if (citor != mShaderNameMap.end())
    {
        outIndex = citor->second;
        return true;
    }
    return false;
}

void shader_mgr_test()
{
    MaterialAttr<XMFLOAT4X4> Local2World(local_to_world);
    MaterialAttr<XMFLOAT3> CamPos(cam_pos);

    std::vector<MaterialAttributeInterface*> tmp;
    tmp.push_back(&Local2World);
    tmp.push_back(&CamPos);

    std::string baseShader ("base");
    Shader* shader = Singleton<ShaderMgr>::getInstance().getShaderByName(baseShader);
    shader->setMaterial(tmp);

    DefaultMaterial baseMat(std::string("base"));
    shader->setMaterial(&baseMat);

    // Check if correctly set
    Shader::ShaderParamMap::iterator it =  shader->mParamMap.find(plain_material);
    MeshMaterial testData;
    if (it != shader->mParamMap.end())
    {
        it->second.mVar->GetRawValue(&testData, 0, sizeof MeshMaterial);
        ;
    }
}
