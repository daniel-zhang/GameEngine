#include "SwapChain.h"
#include "RenderInterface.h"
#include "RenderWindow.h"
#include "D3DUtilities.h"
#include "ConfigMgr.h"

SwapChain::SwapChain()
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::init( RenderInterface* ri, RenderWindow* rw)
{
    /*
    if (mRenderWindow->init(L"test window", 0, 0, 400, 400) == false)
    {
        return false;
    }
    mWidth = mRenderWindow->mWidth;
    mHeight = mRenderWindow->mHeight;
    */

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
    d3d_check(ri->mDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
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



