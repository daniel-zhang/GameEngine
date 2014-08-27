#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "reference.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class RenderWindow;
class SwapChainRT;
class Viewport;
// A thin layer upon d3d11
class RenderInterface
{
public:
    RenderInterface();
    ~RenderInterface();

    bool init();

    // State query
    bool isInitialized();

    // Event handlers
    void resizeViewport(float width, float height);

    // Operations
    void presentBackBuffer();

    /*
    void setRenderTarget(RenderTarget* rt);
    RenderTarget* getRenderTarget();
    void setSwapChain(SwapChainRT* inSwapChain);
    */

    bool createViewport(RenderWindow* rw);
    void setViewport(Viewport* vp);
    void setViewportByIndex(uint32 index);

    void clearBackground(XMFLOAT4& color);

    void setShader();

    void setIndexBuffer(IndexBuffer* pIB){}
    void setVertexBuffer(VertexBuffer* pVB){}

public:
    ID3D11Device*				mDevice;
    ID3D11DeviceContext*		mCtx;

    bool mInitialized;
    //std::vector<Viewport*> mViewports;
    Viewport* mViewport;
};

#endif