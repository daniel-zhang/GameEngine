#ifndef I_RENDERABLE_H 
#define I_RENDERABLE_H

#include <vector>
#include "reference.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class SceneEntity;
class RenderInterface;
class Mesh;
class MaterialInterface;

class IRenderable
{
public:
    virtual void drawSelf(class RenderInterface* ri) = 0;
};

class MeshSection : public IRenderable
{
public:
    virtual void drawSelf(RenderInterface* ri);

public:
    MaterialInterface* mMaterial;

    // Linkages
    uint32 index_start;
    uint32 index_offset;

    Mesh* mMesh;
    SceneEntity* mEntity;
};

class Mesh : public IRenderable
{
public:
    std::vector<MeshSection> sections;

    DynamicVertexBuffer mDynVB;
    IndexBuffer mIB;

public:
    virtual void drawSelf(RenderInterface* ri);
    void setLinkage(SceneEntity* entity);
};

#endif