#include "RenderTarget.h"
#include "RenderWindow.h"
#include "RenderInterface.h"
#include "ConfigMgr.h"
/*
RenderTarget::RenderTarget()
{
    mWidth              = 0;
    mHeight             = 0;
    mDepthStencilView   = NULL;
    mRenderTargetView   = NULL;
    mDepthStencilBuffer = NULL;

    mRI = NULL;
    mIsAttached= false;
}

RenderTarget::~RenderTarget()
{
    safe_release(&mDepthStencilView);
    safe_release(&mRenderTargetView);
    safe_release(&mDepthStencilBuffer);
}

void RenderTarget::attach( RenderInterface* ri )
{
    if (ri->isInitialized())
    {
        ri->mCtx->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

        mRI = ri;
        mIsAttached = true;
    }
}

void RenderTarget::detach()
{
    mIsAttached = false;
    mRI = NULL;
}

void RenderTarget::clear( XMFLOAT4& color )
{
    if (mIsAttached && mRI->isInitialized())
    {
        mRI->mCtx->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&XMLoadFloat4(&color)));
        mRI->mCtx->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
}

Viewport* RenderTarget::getDefaultViewport()
{
    mViewport.resize((float)mWidth, (float)mHeight);
    return &mViewport;
}


///////////////////////////////////////////

SwapChainRT::SwapChainRT()
{
    mRI = NULL;
}

SwapChainRT::~SwapChainRT()
{
}

bool SwapChainRT::init( RenderInterface* ri, RenderWindow* rw)
{
    /*
    if (mRenderWindow->init(L"test window", 0, 0, 400, 400) == false)
    {
        return false;
    }
    mWidth = mRenderWindow->mWidth;
    mHeight = mRenderWindow->mHeight;

    mRI = ri;

    RenderConfig& rc = Singleton<ConfigMgr>::getInstance().root.render_config;
    this->mWidth = rc.screen_width;
    this->mHeight = rc.screen_height;


    uint32 msaaQuality;
    d3d_check(ri->mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaaQuality));
    bool useMSAA = msaaQuality > 0 && rc.enable_msaa;

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width  = mWidth;
    sd.BufferDesc.Height = mHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    // Use 4X MSAA? 
    if(useMSAA)
    {
        sd.SampleDesc.Count   = rc.msaa_quality;
        sd.SampleDesc.Quality = msaaQuality-1;
    }
    // No MSAA
    else
    {
        sd.SampleDesc.Count   = 1;
        sd.SampleDesc.Quality = 0;
    }
    sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount  = 1;
    sd.OutputWindow = rw->getReference();
    sd.Windowed     = true;
    sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags        = 0;

    // To correctly create the swap chain, we must use the IDXGIFactory that was
    // used to create the device.  If we tried to use a different IDXGIFactory instance
    // (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
    // This function is being called with a device from a different IDXGIFactory."
    IDXGIDevice* dxgiDevice = 0;
    d3d_check(ri->mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

    IDXGIAdapter* dxgiAdapter = 0;
    d3d_check(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

    IDXGIFactory* dxgiFactory = 0;
    d3d_check(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

    d3d_check(dxgiFactory->CreateSwapChain(ri->mDevice, &sd, &mSwapChain));

    safe_release(&dxgiDevice);
    safe_release(&dxgiAdapter);
    safe_release(&dxgiFactory);

    safe_release(&mRenderTargetView);
    safe_release(&mDepthStencilView);
    safe_release(&mDepthStencilBuffer);

    //
    // Create RTV
    //
    ID3D11Texture2D* backBuffer;
    d3d_check(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
    d3d_check(mRI->mDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
    safe_release(&backBuffer);

    //
    // Create DSB and DSV
    //
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width     = mWidth;
    depthStencilDesc.Height    = mHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0; 
    depthStencilDesc.MiscFlags      = 0;
    // Use 4X MSAA? --must match swap chain MSAA values.
    if( useMSAA )
    {
        depthStencilDesc.SampleDesc.Count   = rc.msaa_quality;
        depthStencilDesc.SampleDesc.Quality = msaaQuality - 1;
    }
    // No MSAA
    else
    {
        depthStencilDesc.SampleDesc.Count   = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    d3d_check(ri->mDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
    d3d_check(ri->mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));

    return true;
}

void SwapChainRT::swap()
{
    mSwapChain->Present(0, 0);
}
*/

////////////////////////////////////////////////////////////////////////////////////////////

Viewport::Viewport()
{
    mTopLeftX = mTopLeftY = 0.f;
    mWidth = 800.f;
    mHeight = 600.f;
    mMinDepth = 0.f;
    mMaxDepth = 1.f;
    mIsValid = false;
    mBackBuffer = mDepthStencilBuffer = NULL;
    mSwapChain = NULL;

    mRI = NULL;
    mConfig = NULL;
    mWindow = NULL;
}

Viewport::~Viewport()
{
    if (mBackBuffer)
    {
        delete mBackBuffer;
        mBackBuffer = NULL;
    }
    if (mDepthStencilBuffer)
    {
        delete mDepthStencilBuffer;
        mDepthStencilBuffer = NULL;
    }
    safe_release(&mSwapChain);
}

bool Viewport::init( RenderInterface* ri, RenderWindow* rw )
{
    // Prevent multiple init
    if (mIsValid == true)
    {
        return false;
    }
    mRI = ri;
    mWidth = (float)rw->getWidth();
    mHeight = (float)rw->getHeight();
    mMinDepth = 0.f;
    mMaxDepth = 1.f;

    mConfig = &(Singleton<ConfigMgr>::getInstance().root.render_config);
    mWindow = rw->getReference();

    mBackBuffer = new Surface();
    mDepthStencilBuffer = new Surface();
    createSwapChain();
    resize(mWidth, mHeight);

    mIsValid = true;
    return true;
}

void Viewport::resize( float width, float height )
{
    if (width < 0 || height < 0)
        return;

    mWidth = width;
    mHeight = height;

    /*
    From MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb174577%28v=vs.85%29.aspx
        You can't resize a swap chain unless you release all outstanding references to its back buffers. 
        You must release all of its direct and indirect references on the back buffers in order for ResizeBuffers to succeed.
    */
    mBackBuffer->clear();
    mDepthStencilBuffer->clear();
    d3d_check(mSwapChain->ResizeBuffers(1, (uint32)width, (uint32)height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

    //
    // Re-Create RTV 
    //
    d3d_check(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&mBackBuffer->mBuffer)));
    d3d_check(mRI->mDevice->CreateRenderTargetView(mBackBuffer->mBuffer, 0, &mBackBuffer->mRenderTargetView));

    //
    // Re-Create DSV and depth/stencil buffer
    //
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width     = (uint32)width;
    depthStencilDesc.Height    = (uint32)height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0; 
    depthStencilDesc.MiscFlags      = 0;
    // Use 4X MSAA? --must match swap chain MSAA values.
    uint32 msaaQuality = 0;
    if(shouldUseMSAA(msaaQuality))
    {
        depthStencilDesc.SampleDesc.Count   = mConfig->msaa_quality;
        depthStencilDesc.SampleDesc.Quality = msaaQuality - 1;
    }
    // No MSAA
    else
    {
        depthStencilDesc.SampleDesc.Count   = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    d3d_check(mRI->mDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer->mBuffer));
    d3d_check(mRI->mDevice->CreateDepthStencilView(mDepthStencilBuffer->mBuffer, 0, &mDepthStencilBuffer->mDepthStencilView));
}

void Viewport::attach()
{
    // set RTV and DSV 
    mRI->mCtx->OMSetRenderTargets(1, &mBackBuffer->mRenderTargetView, mDepthStencilBuffer->mDepthStencilView);

    // set viewport 
    D3D11_VIEWPORT structVP = {mTopLeftX, mTopLeftY, mWidth, mHeight, mMinDepth, mMaxDepth};
    mRI->mCtx->RSSetViewports(1, &structVP);
}

void Viewport::clear( XMFLOAT4& color )
{
    mRI->mCtx->ClearRenderTargetView(mBackBuffer->mRenderTargetView, reinterpret_cast<const float*>(&XMLoadFloat4(&color)));
    mRI->mCtx->ClearDepthStencilView(mDepthStencilBuffer->mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Viewport::flip()
{
    mSwapChain->Present(0, 0);
}

void Viewport::createSwapChain()
{
    safe_release(&mSwapChain);

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width  = (uint32)mWidth;
    sd.BufferDesc.Height = (uint32)mHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    // Use 4X MSAA? 
    uint32 msaaQuality = 0;
    if(shouldUseMSAA(msaaQuality))
    {
        sd.SampleDesc.Count   = mConfig->msaa_quality;
        sd.SampleDesc.Quality = msaaQuality-1;
    }
    // No MSAA
    else
    {
        sd.SampleDesc.Count   = 1;
        sd.SampleDesc.Quality = 0;
    }
    sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount  = 1;
    sd.OutputWindow = mWindow; // bind swapchain to render window
    sd.Windowed     = true;
    sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags        = 0;

    IDXGIDevice* dxgiDevice = 0;
    d3d_check(mRI->mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

    IDXGIAdapter* dxgiAdapter = 0;
    d3d_check(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

    IDXGIFactory* dxgiFactory = 0;
    d3d_check(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

    d3d_check(dxgiFactory->CreateSwapChain(mRI->mDevice, &sd, &mSwapChain));

    safe_release(&dxgiDevice);
    safe_release(&dxgiAdapter);
    safe_release(&dxgiFactory);
}

bool Viewport::shouldUseMSAA(uint32& msaaQuality)
{
    d3d_check(mRI->mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaaQuality));
    bool useMSAA = msaaQuality > 0 && mConfig->enable_msaa;

    return useMSAA;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Surface::Surface( ID3D11RenderTargetView* inRenderTargetView /*= NULL*/, ID3D11DepthStencilView* inDepthStencilView /*= NULL*/, ID3D11ShaderResourceView* inShaderResourceView /*= NULL*/, ID3D11UnorderedAccessView* inUnorderedAccessView /*= NULL*/, ID3D11Texture2D* inBuffer /*= NULL*/ ) :
    mRenderTargetView(inRenderTargetView),
    mDepthStencilView(inDepthStencilView),
    mShaderResourceView(inShaderResourceView),
    mUnorderedAccessView(inUnorderedAccessView),
    mBuffer(inBuffer) {}

Surface::~Surface()
{
    clear();
}

void Surface::clear()
{
    safe_release(&mRenderTargetView);
    safe_release(&mDepthStencilView);
    safe_release(&mShaderResourceView);
    safe_release(&mUnorderedAccessView);
    safe_release(&mBuffer);
}
