#ifndef RENDER_STATE_MGR
#define RENDER_STATE_MGR

#include "reference.h"

class RenderStateMgr
{
public:
    enum ERenderState
    {
        RS_Wireframe = 0,
        RS_NoCull
    };

    bool init();
    bool destory();
};

class RenderState
{
public:
    virtual void bindToRI(class RenderInterface* ri) = 0;
};

class WireframeRS : public RenderState
{
public:
    void bindToRI(class RenderInterface* ri);

protected:
    ID3D11RasterizerState* mRenderState;
};


#endif