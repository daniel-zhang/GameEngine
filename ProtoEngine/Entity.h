#ifndef ENTITY_H 
#define ENTITY_H 

//#include "reference.h"
#include "ProtoMath.h"

class IControllable
{
public:
    virtual void walk( float d );
    virtual void strafe( float d );
    virtual void pitch( float d );
    virtual void yaw( float d );
    virtual void roll( float d );

    const XMFLOAT3& getTranslation(){return mTranslation;}
    const XMFLOAT3& getScaling(){return mScaling;}
    const XMFLOAT3 getRotation(){return XMFLOAT3();}
protected:
    XMFLOAT3 mTranslation;
    XMFLOAT3 mScaling;
    Rotator mRotation;
};

/*
Though it is called "entity", this is class is not that generic at the time being.
It acts more like a Mesh Instance for now.
Being able to attach all game objects, though very exciting, are way too beyond the current requirement.
*/
class Mesh;
class Entity : public IControllable
{
public:
    void attach(Mesh* mesh);
    void detach();

    // Game loop call this one
    void update(float delta);
    // Render loop call this one
    void draw();

public:
    Mesh* mMesh;
};

#endif