#ifndef ENTITY_H 
#define ENTITY_H 

#include "reference.h"

class Mesh;

class Entity
{
public:
    void attach(Mesh* mesh);
    void detach();

public:
    Mesh* mMesh;

    XMFLOAT3 mTranslation;
    XMFLOAT3 mRotation;
    XMFLOAT3 mScaling;
};

#endif