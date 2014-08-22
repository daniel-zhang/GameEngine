#include "Entity.h"
#include "Renderable.h"

void Entity::attach( Mesh* mesh )
{
    mMesh = mesh;
    mMesh->setLinkage(this);
}
