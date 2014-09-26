#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <string>

class Mesh;
class MeshAccessor 
{
public:
    void loadFromObj(std::wstring& objFileName, Mesh& mesh);

};

// A thin wrapper around the DirectX Tex library


#endif