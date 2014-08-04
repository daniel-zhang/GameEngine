#include "RenderInterface.h"
#include <Windows.h>

RenderInterface::RenderInterface()
{
    mDevice = NULL;
    mCtx = NULL;
    mInitialized = false;
}

RenderInterface::~RenderInterface()
{
    if (mCtx) mCtx->ClearState(); 
    safe_release(&mCtx);
    safe_release(&mDevice);
}

bool RenderInterface::init(RenderConfig& config)
{
    D3D_FEATURE_LEVEL featureLevel;
    UINT createDeviceFlags = 0;
    if(config.ForceSingleThreaded) createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDevice(
        0,                 // default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        0,                 // no software device
        createDeviceFlags, 
        0, 0,              // default feature level array
        D3D11_SDK_VERSION,
        &mDevice,
        &featureLevel,
        &mCtx);

    if( FAILED(hr) )
    {
        MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
        return false;
    }

    if( featureLevel != D3D_FEATURE_LEVEL_11_0 )
    {
        MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
        return false;
    }

    mInitialized = true;
    return mInitialized;
}

void RenderInterface::attachRenderTarget( RenderTarget* rt )
{
    if (rt)
    {
        mCtx->OMSetRenderTargets(1, &rt->mRenderTargetView, rt->mDepthStencilView);
    }
}

bool RenderInterface::isInitialized()
{
    return mInitialized;
}

