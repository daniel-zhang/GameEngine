#include "DebugPrimitiveGenerator.h"
#include <vector>

void DebugPrimitiveGenerator::createBox( float width, float height, float depth, Mesh& inMesh )
{
    typedef VertexFactory<e_pos_normal_tan_tex> Vertex;
    std::vector<Vertex> verts;
    verts.resize(24);

    float w2 = 0.5f*width;
    float h2 = 0.5f*height;
    float d2 = 0.5f*depth;

    // Fill in the front face vertex data.
    verts[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    verts[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    verts[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    verts[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // Fill in the back face vertex data.
    verts[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    verts[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    verts[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    verts[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Fill in the top face vertex data.
    verts[8]  = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    verts[9]  = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    verts[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    verts[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // Fill in the bottom face vertex data.
    verts[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    verts[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    verts[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    verts[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Fill in the left face vertex data.
    verts[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    verts[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    verts[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
    verts[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

    // Fill in the right face vertex data.
    verts[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    verts[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    verts[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    verts[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    //inMesh.mVertexBuffer.getLocalCache().assign(&v[0], &v[24]);
    inMesh.mVertexBuffer.setVertArray(verts);

    std::vector<uint32> indices;
    indices.resize(36);

    // Fill in the front face index data
    indices[0] = 0; indices[1] = 1; indices[2] = 2;
    indices[3] = 0; indices[4] = 2; indices[5] = 3;

    // Fill in the back face index data
    indices[6] = 4; indices[7]  = 5; indices[8]  = 6;
    indices[9] = 4; indices[10] = 6; indices[11] = 7;

    // Fill in the top face index data
    indices[12] = 8; indices[13] =  9; indices[14] = 10;
    indices[15] = 8; indices[16] = 10; indices[17] = 11;

    // Fill in the bottom face index data
    indices[18] = 12; indices[19] = 13; indices[20] = 14;
    indices[21] = 12; indices[22] = 14; indices[23] = 15;

    // Fill in the left face index data
    indices[24] = 16; indices[25] = 17; indices[26] = 18;
    indices[27] = 16; indices[28] = 18; indices[29] = 19;

    // Fill in the right face index data
    indices[30] = 20; indices[31] = 21; indices[32] = 22;
    indices[33] = 20; indices[34] = 22; indices[35] = 23;

    inMesh.mIndexBuffer.setIndArray(indices);
    //void addSubMesh(std::string& defaultShaderName, uint32 indexCount, uint32 indexOffset)

    inMesh.addSubMesh(std::string("base"), 36, 0);
}

void DebugPrimitiveGenerator::createGrid( float width, float depth, uint32 rows, uint32 cols, Mesh& inMesh )
{
    typedef VertexFactory<e_pos_normal_tan_tex> Vertex;

    uint32 vertexCount = rows*cols;
    uint32 faceCount   = (rows-1)*(cols-1)*2;
    std::vector<Vertex> verts;
    verts.resize(vertexCount);

    // Create the verts
    float halfWidth = 0.5f*width;
    float halfDepth = 0.5f*depth;

    float dx = width / (cols-1);
    float dz = depth / (rows-1);

    float du = 1.0f / (cols-1);
    float dv = 1.0f / (rows-1);

    for(UINT i = 0; i < rows; ++i)
    {
        float z = halfDepth - i*dz;
        for(UINT j = 0; j < cols; ++j)
        {
            float x = -halfWidth + j*dx;

            verts[i*cols+j].position = XMFLOAT3(x, 0.0f, z);
            verts[i*cols+j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
            verts[i*cols+j].tangent= XMFLOAT3(1.0f, 0.0f, 0.0f);

            // Stretch texture over grid.
            verts[i*cols+j].texcoord.x = j*du;
            verts[i*cols+j].texcoord.y = i*dv;
        }
    }

    inMesh.mVertexBuffer.setVertArray(verts);

    // Create the indices.
    std::vector<uint32> indices;
    indices.resize(faceCount*3);

    // Iterate over each quad and compute indices.
    UINT k = 0;
    for(UINT i = 0; i < rows-1; ++i)
    {
        for(UINT j = 0; j < cols-1; ++j)
        {
            indices[k]   = i*cols+j;
            indices[k+1] = i*cols+j+1;
            indices[k+2] = (i+1)*cols+j;

            indices[k+3] = (i+1)*cols+j;
            indices[k+4] = i*cols+j+1;
            indices[k+5] = (i+1)*cols+j+1;

            k += 6; // next quad
        }
    }
    inMesh.mIndexBuffer.setIndArray(indices);

    inMesh.addSubMesh(std::string("base"), faceCount*3, 0);
}
