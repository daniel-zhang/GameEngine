#include "RenderInterface.h"
#include "RenderTarget.h"
#include <Windows.h>

RenderInterface::RenderInterface()
{
    mDevice = NULL;
    mCtx = NULL;
    mViewport = NULL;
    mInitialized = false;
    
    mWireframe = NULL;
    mNoBackfaceCulling = NULL;
    mWireframe_NoBackfaceCulling = NULL;

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

    safe_release(&mWireframe);
    safe_release(&mNoBackfaceCulling);
    safe_release(&mWireframe_NoBackfaceCulling);
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

    // Create wireframe render state
    D3D11_RASTERIZER_DESC wireframeDesc;
    ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
    wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
    wireframeDesc.CullMode = D3D11_CULL_BACK;
    wireframeDesc.FrontCounterClockwise = false;
    wireframeDesc.DepthClipEnable = true;
    d3d_check(mDevice->CreateRasterizerState(&wireframeDesc, &mWireframe));

    // Create no-backface-culling render state
    D3D11_RASTERIZER_DESC noCullDesc;
    ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
    noCullDesc.FillMode = D3D11_FILL_SOLID;
    noCullDesc.CullMode = D3D11_CULL_NONE;
    noCullDesc.FrontCounterClockwise = false;
    noCullDesc.DepthClipEnable = true;
    d3d_check(mDevice->CreateRasterizerState(&noCullDesc, &mNoBackfaceCulling));

    // Create no-backface-culling + wireframe render state
    D3D11_RASTERIZER_DESC noCullDesc;
    ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
    noCullDesc.FillMode = D3D11_FILL_WIREFRAME;
    noCullDesc.CullMode = D3D11_CULL_NONE;
    noCullDesc.FrontCounterClockwise = false;
    noCullDesc.DepthClipEnable = true;
    d3d_check(mDevice->CreateRasterizerState(&noCullDesc, &mNoBackfaceCulling));

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

void RenderInterface::resizeViewport( float width, float height )
{
    if (mViewport && mViewport->isValid()) 
    {
        mViewport->resize(width, height);
        mViewport->attach();
    }
}

void RenderInterface::drawIndexedPrimtive( uint32 indexCount, uint32 indexOffset )
{
    // TODO: add a draw call counter here
    mCtx->DrawIndexed(indexCount, indexOffset, 0);
}

float RenderInterface::getViewportAspect()
{
    return mViewport->getAspect();
}


