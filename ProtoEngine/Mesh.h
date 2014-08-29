#ifndef MESH_H
#define MESH_H

#include <vector>
#include "reference.h"
#include "GraphicBuffer.h"

class Entity;
class RenderInterface;
class Mesh;
class MaterialInterface;

class SubMesh 
{
public:
    void drawSelf(RenderInterface* ri);

protected:
    MaterialInterface* mMaterial;

    // Linkages
    uint32 index_start;
    uint32 index_offset;

    Mesh* mParentMesh;
    Entity* mHostingEntity;
};

class Mesh 
{
public:
    void drawSelf(RenderInterface* ri);
    void setLinkage(Entity* entity);

protected:
    Entity* mHostingEntity;
    std::vector<SubMesh*> mSubMeshes;

    IndexBuffer mIndexBuffer;
    VertexBuffer<e_pos_normal_tex> mVertexBuffer;
};

#endif