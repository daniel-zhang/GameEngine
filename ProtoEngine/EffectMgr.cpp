#include "EffectMgr.h"
#include "Effect.h"
#include "RenderInterface.h"
#include "GameResourcePath.h"
#include "D3DUtilities.h"
#include "OSInterface.h"

#include <D3Dcompiler.h>
#include <d3dx11.h>

class EffectInspector
{
public:
    EffectInspector(){mFX = NULL;}
    EffectInspector(ID3DX11Effect* fx):mFX(fx){}

    void go()
    {
        if(mFX == NULL)
            return;
        /*
        UINT    ConstantBuffers;        // Number of constant buffers in this effect
        UINT    GlobalVariables;        // Number of global variables in this effect
        UINT    InterfaceVariables;     // Number of global interfaces in this effect
        UINT    Techniques;             // Number of techniques in this effect
        UINT    Groups;                 // Number of groups in this effect
        */
        D3DX11_EFFECT_DESC desc;
        mFX->GetDesc(&desc);

        for (uint32  i  = 0; i < desc.ConstantBuffers; ++i)
        {
            mConstBuffers.push_back(mFX->GetConstantBufferByIndex(i));
            D3DX11_EFFECT_VARIABLE_DESC cbufferDesc;
            mConstBuffers[i]->GetDesc(&cbufferDesc);
            mConstBufferDescs.push_back(cbufferDesc);
        }

        for (uint32 i = 0; i < desc.GlobalVariables; ++i)
        {
            mGlobalVars.push_back(mFX->GetVariableByIndex(i));

            D3DX11_EFFECT_VARIABLE_DESC varDesc;
            mGlobalVars[i]->GetDesc(&varDesc);
            mGlobalVarDescs.push_back(varDesc);
        }

        for (uint32  i  = 0; i < desc.Techniques; ++i)
        {
            mTechs.push_back(mFX->GetTechniqueByIndex(i));
            D3DX11_TECHNIQUE_DESC techDesc;
            mTechs[i]->GetDesc(&techDesc);
            mTechDescs.push_back(techDesc);
            for(uint32 j = 0; j < techDesc.Passes; ++j)
            {
                mPasses.push_back(mTechs[i]->GetPassByIndex(j));
                D3DX11_PASS_DESC passDesc;
                mPasses[mPasses.size() - 1]->GetDesc(&passDesc);
                mPassDescs.push_back(passDesc);
            }
        }
    }

protected:
    ID3DX11Effect* mFX;
    std::vector<ID3DX11EffectVariable*> mGlobalVars;
    std::vector<D3DX11_EFFECT_VARIABLE_DESC> mGlobalVarDescs;

    std::vector<ID3DX11EffectConstantBuffer*> mConstBuffers;
    std::vector<D3DX11_EFFECT_VARIABLE_DESC> mConstBufferDescs;

    std::vector<ID3DX11EffectTechnique*> mTechs;
    std::vector<D3DX11_TECHNIQUE_DESC> mTechDescs;

    std::vector<ID3DX11EffectPass*> mPasses;
    std::vector<D3DX11_PASS_DESC> mPassDescs;
};

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
    std::string path = gShaderPath + std::string("basic.fxo");
    OSInterface::fileToMem(path, rawMem );

    ID3DX11Effect* effect = NULL;
    d3d_check( 
        D3DX11CreateEffectFromMemory(rawMem.mData, rawMem.mSize, 0, ri->mDevice, &effect)
        );
    ///////////////////////////////////////////////////////////////////////
    // Test begin

    EffectInspector ei(effect);
    ei.go();

    // Test end
    ///////////////////////////////////////////////////////////////////////

    return true;
}

bool EffectMgr::destroy()
{
    return true;
}

Effect* EffectMgr::getByName( const char* effectName )
{

    return NULL;
}

Effect* EffectMgr::getByIndex()
{
    return NULL;

}
