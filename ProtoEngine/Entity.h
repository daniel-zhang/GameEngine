#ifndef ENTITY_H 
#define ENTITY_H 

//#include "reference.h"
#include "ProtoMath.h"
#include "ShaderDataReference.h"
#include "RenderInterface.h"

class Scene;

class IControllable
{
public:
    IControllable()
    {
        mIsActive = false;
        mTranslation = XMFLOAT3(0.f, 0.f, 0.f);
        mScaling = XMFLOAT3(1.f, 1.f, 1.f);
    }
    virtual void walk( float d );
    virtual void strafe( float d );
    virtual void pitch( float d );
    virtual void yaw( float d );
    virtual void roll( float d );
    
    void setActive(){mIsActive = true;}
    void setInative(){mIsActive = false;}
    bool isActive(){return mIsActive;}

    const XMFLOAT3& getTranslation(){return mTranslation;}
    const XMFLOAT3& getScaling(){return mScaling;}
    const XMFLOAT3 getRotation(){return XMFLOAT3();}
    /*
    XMFLOAT3& getTranslation(){return mTranslation;}
    XMFLOAT3& getScaling(){return mScaling;}
    XMFLOAT3 getRotation(){return XMFLOAT3();}
    */
protected:
    bool mIsActive;
    XMFLOAT3 mTranslation;
    XMFLOAT3 mScaling;
    Rotator mRotation;
};

/*
Though it is called "entity", this is class is not that generic at the time being.
It acts more like a Mesh Instance for now.
Being able to attach all game objects, though very exciting, are way beyond the current requirement.
*/
class Mesh;
class Entity : public IControllable
{
public:
    Entity();
    void attach(Mesh* mesh, RenderInterface* ri);
    void detach() {} 

    void update(float delta);
    void drawSelf(RenderInterface* ri); 

    void buildEntityShaderData();
    ShaderDataReference& getEntityShaderData();
    Scene* getSceneRef();
    void setSceneRef(Scene* inScene);

protected:
    friend class scene;
    ShaderDataReference mEntityShaderData;
    XMFLOAT4X4 mLocalToWorld;
    XMFLOAT4X4 mInvertTranpose_LocalToWorld;
    Mesh* mMeshRef;
    Scene* mSceneRef;
};

#endif