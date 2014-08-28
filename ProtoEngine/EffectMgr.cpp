#include "EffectMgr.h"
#include "RenderInterface.h"
#include "Material.h"

#include "OSInterface.h"
#include "GameResourcePath.h"

#include <D3Dcompiler.h>
#include <d3dx11.h>

/*
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

MetaEffect* EffectMgr::getByName( const char* effectName )
{

    return NULL;
}

MetaEffect* EffectMgr::getByIndex()
{
    return NULL;

}
*/

//
//
//

// TODO 
// shader names should be read from config or scanned from shader dir
const char* gEffectName[] = {
    "base"
};

EffectMgr::EffectMgr()
{

}

EffectMgr::~EffectMgr()
{
}

bool EffectMgr::init( RenderInterface* ri )
{
    buildEffectNameMap();
    // Load shaders
    for (uint32 i = 0; i < mEffectNames.size(); ++i)
    {
        std::string fullname = gShaderPath + mEffectNames[i] + ".fxo";
        MemBlock rawMem;
        if ( OSInterface::fileToMem(fullname, rawMem ) == false )
        {
            // throw error 
        }
        ID3DX11Effect* fx= NULL;
        d3d_check( D3DX11CreateEffectFromMemory(rawMem.mData, rawMem.mSize, 0, ri->mDevice, &fx) );

        ShaderEffect* effect = new ShaderEffect;
        if(effect->init(ri, fx, &mTagDefinition))
            mEffects.push_back(effect);
        else
            ; //throw error
    }
    return true;
}

bool EffectMgr::destroy()
{
    for (uint32 i = 0; i < mEffects.size(); ++i)
    {
        if (mEffects[i])
            delete mEffects[i];
    }
    mEffects.clear();
    mEffectNames.clear();
    mEffectNameMap.clear();

    return true;
}

ShaderEffect* EffectMgr::getEffect( std::string& inEffectName )
{
    uint32 effectIndex = 0;
    if (effectNameToIndex(inEffectName, effectIndex) == false)
        return NULL;

    return mEffects[effectIndex];
}

void EffectMgr::buildEffectNameMap()
{
    mEffectNameMap.clear();
    uint32 numShader = ARRAYSIZE(gEffectName);
    for (uint32 i = 0; i < numShader; ++i)
    {
        mEffectNames.push_back(std::string(gEffectName[i]));
        mEffectNameMap.insert(std::pair<std::string, uint32>(std::string(gEffectName[i]), i));
    }
}

bool EffectMgr::effectNameToIndex( const std::string& inEffectName, uint32& outIndex)
{
    TEffectNameMap::const_iterator citor = mEffectNameMap.find(inEffectName);
    if (citor != mEffectNameMap.end())
    {
        outIndex = citor->second;
        return true;
    }
    return false;
}

#include <cassert>
#include <exception>

// In debug mode, throwing this exception will first trigger a breakpoint
// This side effect is taken off in release mode
class SmartException: public std::runtime_error
{
public:


};
void effect_mgr_test()
{
    /*
    MaterialAttr<XMFLOAT4X4> Local2World(e_local_to_world);
    MaterialAttr<XMFLOAT3> CamPos(e_cam_pos);

    std::vector<MaterialAttributeInterface*> tmp;
    tmp.push_back(&Local2World);
    tmp.push_back(&CamPos);
    */

    //assert(0);
    //DebugBreak();
    //d3d_check(D3D11_ERROR_FILE_NOT_FOUND);
    DefaultMaterial baseMat(std::string("base"));

    std::string effectName("base");
    ShaderEffect* effect = Singleton<EffectMgr>::getInstance().getEffect(effectName);
    ShaderInterface* shader = new ShaderInterface;
    shader->init(effectName, std::string("BaseTech"), 0);
    shader->setMaterial(&baseMat);
}
