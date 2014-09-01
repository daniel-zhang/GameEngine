#ifndef DEBUG_PRIMITIVE_GENERATOR
#define DEBUG_PRIMITIVE_GENERATOR

#include "GraphicBuffer.h"
#include "Mesh.h"
#include "reference.h"

class DebugPrimitiveGenerator
{
public:
    void createBox(float width, float height, float depth, Mesh& inMesh);
    void createGrid(float width, float depth, uint32 rows, uint32 cols, Mesh& inMesh);
};

#endif