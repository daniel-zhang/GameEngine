#include "RenderInterface.h"
#include "RenderTarget.h"
#include <Windows.h>

RenderInterface::RenderInterface()
{
    mDevice = NULL;
    mCtx = NULL;
    mViewport = NULL;
    mInitialized = false;
}

RenderInterface::~RenderInterface()
{
    if (mCtx) mCtx->ClearState(); 
    if (mViewport)
    {
        delete mViewport;
        mViewport = NULL;
    }
    safe_release(&mCtx);
    safe_release(&mDevice);
    /*
    for (uint32 i = 0; i < mViewports.size(); ++i)
    {
        if (mViewports[i])
        {
            delete mViewports[i];
        }
    }
    mViewports.clear();
    */
}

bool RenderInterface::init()
{
    D3D_FEATURE_LEVEL featureLevel;
    UINT createDeviceFlags = 0;
    //if(config.ForceSingleThreaded) createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
    if(1) createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
#if 1
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif

    d3d_check(D3D11CreateDevice(
        0,                 // default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        0,                 // no software device
        createDeviceFlags, 
        0, 0,              // default feature level array
        D3D11_SDK_VERSION,
        &mDevice,
        &featureLevel,
        &mCtx));

    mInitialized = true;
    return mInitialized;
}

bool RenderInterface::isInitialized()
{
    return mInitialized;
}
/*
void RenderInterface::setRenderTarget( RenderTarget* rt )
{
    if (rt)
    {
        if (mActiveRenderTarget)
            mActiveRenderTarget->detach();
        mActiveRenderTarget = rt;
        mActiveRenderTarget->attach(this);
    }
}

RenderTarget* RenderInterface::getRenderTarget()
{
    return mActiveRenderTarget;
}
*/

void RenderInterface::clearBackground( XMFLOAT4& color )
{
    mViewport->clear(color);
}

void RenderInterface::presentBackBuffer()
{
    mViewport->flip();
}

/*
void RenderInterface::setSwapChain( SwapChainRT* inSwapChain )
{
    mSwapChain = inSwapChain;
}
*/

bool RenderInterface::createViewport( RenderWindow* rw )
{
    if (mViewport)
    {
        delete mViewport;
        mViewport = NULL;
    }

    mViewport = new Viewport();
    if (mViewport && mViewport->init(this, rw))
    {
        mViewport->attach();
        return true;
    }

    return false;
}

void RenderInterface::setViewport(Viewport* vp)
{
    /*
    vp->attach();
    */
}

void RenderInterface::setViewportByIndex( uint32 index )
{
    /*
    if (index < mViewports.size() && index >= 0)
        mViewports[index]->attach();
        */
}


