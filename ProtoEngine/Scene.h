#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <list>
#include "ProtoMath.h"
#include "Mesh.h"
#include "Camera.h"
#include "ShaderDataReference.h"

class Entity;
class Scene;
class RenderInterface;

void buildDefaultScene(Scene* scene);

// Act as a scoped collection structure
// T is required to have a default ctor
template <typename T>
class Collection 
{
public:
    ~Collection()
    {
        for (uint32 i = 0; i < mStorage.size(); ++i)
        {
            delete mStorage[i];
            mStorage[i] = NULL;
        }
    }
    T* create()
    {
        T* p = new T();
        mStorage.push_back(p);
        return p;
    }
    std::vector<T*> mStorage;
};

/*
a flattened scene structure
no spatial culling
*/
class Scene
{
public:
    typedef void (*T_BuildScene)(Scene*);
	Scene();
	~Scene();

    void initFromFile(const char* filename){}
    void initFromBuilder(T_BuildScene builder, RenderInterface* ri);

    void setBuilder(T_BuildScene pFunc) { mpFunc = pFunc; }
    void execBuilder() { if (mpFunc) (*mpFunc)(this); }
    Entity* createEmptyEntity();
    Mesh* createEmptyMesh();

    // Scene shader data reference
    void buildSceneShaderData();
    ShaderDataReference& getSceneShaderData();

	void addEntity(Entity* entity);
	//void remove(Entity* pNode);
    void update(float delta);
    void drawSelf(RenderInterface* ri);

    RenderInterface* getRenderInterface(){return mRI;}

    Camera& getActiveCam(){return mCam;}

protected:
    RenderInterface* mRI;
    bool mIntialized;
    T_BuildScene mpFunc;
    ShaderDataReference mSceneShaderData;
    Camera mCam;
    DirLight mMainLight;

    friend class Entity;
	std::vector<Entity*> mEntities;

    Collection<Mesh> mMeshRepo;
    Collection<Entity> mEntityRepo;
};

#endif