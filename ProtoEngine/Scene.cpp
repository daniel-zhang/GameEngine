#include "Scene.h"
#include "Camera.h"
#include "Entity.h"

void buildDefaultScene( Scene* scene )
{

}

/////////////////////////////////////////////////////////////
Scene::Scene()
{
    mpFunc = NULL;
    mCam = NULL;
}

Scene::~Scene()
{
}


void Scene::add( Entity* pNode )
{

}

/////////////////////////////////////////////////////////////

SceneBuilder::SceneBuilder()
{
}

SceneBuilder::~SceneBuilder()
{
    std::list<Scene*>::iterator it;
    for (it = mScenes.begin(); it != mScenes.end(); ++it)
    {
        if ((*it) != NULL)
        {
            delete (*it);
            (*it) = NULL;
        }
    }
    mScenes.clear();
}

Scene* SceneBuilder::create()
{
    Scene* scene = new Scene();
    scene->setBuilder(&buildDefaultScene);
    scene->buildOnce();
    mScenes.push_back(scene);
    return scene;
}

bool SceneBuilder::remove( Scene* scene )
{
    std::list<Scene*>::iterator it;
    for (it = mScenes.begin(); it != mScenes.end(); ++it)
    {
        if (scene && (*it) == scene)
        {
            delete (*it);
            (*it) = NULL;
            mScenes.erase(it);
            return true;
        }
    }
    return false;
}

