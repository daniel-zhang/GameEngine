#include "Entity.h"
#include "Renderable.h"

void SceneEntity::attach( Mesh* mesh )
{
    mMesh = mesh;
    mMesh->setLinkage(this);
}
