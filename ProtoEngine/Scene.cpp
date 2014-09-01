#include "Scene.h"
#include "Entity.h"
#include "RenderInterface.h"
#include "DebugPrimitiveGenerator.h"

void buildDefaultScene( Scene* scene )
{
    DebugPrimitiveGenerator dpGen;

    Mesh* pMesh = NULL;
    Entity* pEntity = NULL;
    
    /*
    pMesh = scene->createEmptyMesh();
    dpGen.createGrid(20.f, 30.f, 60, 40, *pMesh);
    pEntity = scene->createEmptyEntity();
    pEntity->attach(pMesh, scene->getRenderInterface());
    scene->addEntity(pEntity);
    */

    pMesh = scene->createEmptyMesh();
    dpGen.createBox(1.f, 1.f, 1.f, *pMesh);
    pEntity = scene->createEmptyEntity();
    pEntity->attach(pMesh, scene->getRenderInterface());
    scene->addEntity(pEntity);

}

/////////////////////////////////////////////////////////////
Scene::Scene()
{
    mpFunc = NULL;
}

Scene::~Scene()
{
}

void Scene::drawSelf( RenderInterface* ri )
{
    ri->clearBackground(XMFLOAT4(0.69f, 0.77f, 0.87f, 1.0f));

    for (uint32 i  = 0; i < mEntities.size(); ++i)
    {
        mEntities[i]->drawSelf(ri);
    }

    ri->presentBackBuffer();
}

void Scene::initFromBuilder( T_BuildScene builder, RenderInterface* ri )
{
    mRI = ri;
    // Add entities/meshes
    setBuilder(builder);
    execBuilder();

    // Init default cam: should be done by SceneBuilder
    mCam.init(XMFLOAT3(0.f, 2.f, -15.f),ri->getViewportAspect(), 0.25f*ProtoMath::Pi, 1.f, 1000.f);

    // Init default main light, should be done by SceneBuilder
    mMainLight.ambient  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mMainLight.diffuse  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mMainLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mMainLight.direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

    // Build shader data reference 
    buildSceneShaderData();
}

Entity* Scene::createEmptyEntity()
{
    return mEntityRepo.create();
}

Mesh* Scene::createEmptyMesh()
{
    return mMeshRepo.create();
}

void Scene::addEntity( Entity* entity )
{
    entity->setSceneRef(this);
    mEntities.push_back(entity);
}

ShaderDataReference& Scene::getSceneShaderData()
{
    return mSceneShaderData;
}

void Scene::buildSceneShaderData()
{
    // Cam
    mSceneShaderData.add<e_cam_pos>(mCam.getTranslation());
    mSceneShaderData.add<e_world_to_view>(mCam.getView());
    mSceneShaderData.add<e_view_to_proj>(mCam.getProj());

    // Lights
    mSceneShaderData.add<e_main_light>(mMainLight);
}

void Scene::update( float delta )
{
    // Update cam
    mCam.updateView();

    // Update entities
    for (uint32 i = 0; i < mEntities.size(); ++i)
    {
        mEntities[i]->update(delta);
    }
}



