#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "reference.h"

class RenderWindow;
class RenderInterface;
class RenderConfig;

class Surface
{
public:
    Surface(
        ID3D11RenderTargetView* inRenderTargetView = NULL,
        ID3D11DepthStencilView* inDepthStencilView       = NULL,
        ID3D11ShaderResourceView* inShaderResourceView   = NULL,
        ID3D11UnorderedAccessView* inUnorderedAccessView = NULL,
        ID3D11Texture2D* inBuffer                        = NULL);
    ~Surface();

    void clear();

    ID3D11RenderTargetView* mRenderTargetView;
    ID3D11DepthStencilView* mDepthStencilView;
    ID3D11ShaderResourceView* mShaderResourceView;
    ID3D11UnorderedAccessView* mUnorderedAccessView;
    ID3D11Texture2D* mBuffer;
};

/*
TODO: Re-factor RenderTarget/Viewport organization

RenderTarget:
    resize();
    clearBackground(color);
    addViewport(viewport, index);
    setAllViewports();
    resizeViewportByIndex(index);
    flip();

    vector<Viewport> mViewports;
*/
class Viewport
{
public:
    Viewport();
    ~Viewport();

    bool isValid(){return mIsValid;}
    bool init(RenderInterface* ri, RenderWindow* rw);

    void attach();
    void resize(float width, float height);
    void clear(XMFLOAT4& color);
    void flip();

    /*
    Surface* getBackBuffer();
    Surface* getDepthStencilBuffer();
    */
protected:
    void createSwapChain();
    bool shouldUseMSAA(uint32& quality);

protected:
    float mTopLeftX;
    float mTopLeftY;
    float mWidth;
    float mHeight;
    float mMinDepth;
    float mMaxDepth;

    bool mIsValid;

    // Owned
    IDXGISwapChain* mSwapChain;
    Surface* mBackBuffer;
    Surface* mDepthStencilBuffer;

    // Referenced
    RenderInterface* mRI;
    RenderConfig* mConfig;
    HWND mWindow;
};

/*
class RenderTarget
{
public:
    RenderTarget();
    virtual ~RenderTarget();

    virtual void resize(uint32 width, uint32 height);
    virtual void attach(RenderInterface* ri);
    virtual void detach();
    virtual void clear(XMFLOAT4& color);
    
    Viewport* getDefaultViewport();
    uint32 getWidth(){return mWidth;}
    uint32 getHeight(){return mHeight;}

public:
    RenderInterface* mRI;
    bool mIsAttached;

    uint32 mWidth;
    uint32 mHeight;

    ID3D11DepthStencilView*		mDepthStencilView;
    ID3D11RenderTargetView*		mRenderTargetView;
    ID3D11Texture2D*			mDepthStencilBuffer;
    Viewport mViewport;
};

class SwapChainRT: public RenderTarget
{
public:
    SwapChainRT();
    ~SwapChainRT();
    bool init(RenderInterface* ri, RenderWindow* rw);
    void swap();

protected:
    bool mInitialized;
    IDXGISwapChain* mSwapChain;
};
*/


#endif