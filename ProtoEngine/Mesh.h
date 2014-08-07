#ifndef MESH_H
#define MESH_H

#include <vector>


class Effect
{
public:


};

class Texture
{
public:

};

class Material
{
public:
    void setParam();
    void apply(RenderInterface* ri);

public:
    uint32 mEffectId;
    uint32 mTechId;
    std::vector<Texture*> mTextures;
    // Sampler state
    XMFLOAT4 ambient;
};

class MeshSection
{
public:
    Material mat;

    uint32 index_start;
    uint32 index_offset;
};

class Mesh
{
public:
    vector<MeshSection> sections;
    XMFLOAT4X4 local2world;

    VertexBuffer vb;
    IndexBuffer ib;

public:
    void drawSelf(RenderInterface* ri)
    {
        for (uint32 i = 0; i < sections.size(); ++i)
        {
            ri->setIndexBuffer(&ib);
            ri->setVertexBuffer(&vb);

            sections[i].mat.setParam(local2world);
        }
    }
};

#endif