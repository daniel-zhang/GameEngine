#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <list>
#include "ProtoMath.h"
//#include "reference.h"

class Camera;
class Entity;
class Scene;

void buildDefaultScene(Scene* scene);

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

    void setBuilder(T_BuildScene pFunc) { mpFunc = pFunc; }
    void buildOnce() { if (mpFunc) (*mpFunc)(this); }
	void add(Entity* pNode);
	//void remove(Entity* pNode);

    void update(float delta);
    Camera* getCam(){return mCam;}

public:
    bool mIntialized;
    T_BuildScene mpFunc;

    Camera* mCam;
	std::vector<Entity*> mNodes;
};

class SceneBuilder
{
public:
    SceneBuilder();
    ~SceneBuilder();

    Scene* create();
    bool remove(Scene* scene);

public:
    std::list<Scene*> mScenes;
};


#endif