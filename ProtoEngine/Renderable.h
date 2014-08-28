#ifndef I_RENDERABLE_H 
#define I_RENDERABLE_H

#include <vector>
#include "reference.h"
#include "GraphicBuffer.h"

class Entity;
class RenderInterface;
class Mesh;
class MaterialInterface;

class MeshSection 
{
public:
    void drawSelf(RenderInterface* ri);

protected:
    MaterialInterface* mMaterial;

    // Linkages
    uint32 index_start;
    uint32 index_offset;

    Mesh* mMesh;
    Entity* mEntity;
};

class Mesh 
{
public:
    void drawSelf(RenderInterface* ri);
    void setLinkage(Entity* entity);

protected:
    std::vector<MeshSection> sections;

    /*
    DynamicVertexBuffer mDynVB;
    IndexBuffer mIB;
    */
};

#endif