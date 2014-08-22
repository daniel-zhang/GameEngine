#include "Renderable.h"
#include "Entity.h"
#include "RenderInterface.h"
#include "Material.h"

void MeshSection::drawSelf( RenderInterface* ri )
{
    // set light
    // set cam
    // set fog

    // set input layout
    // set primitive topo
    // set vertex buffer

    //mMaterial->apply(ri, mEntity);

    // draw()
}

void Mesh::drawSelf( RenderInterface* ri )
{
    ri->setIndexBuffer(&mIB);
    ri->setVertexBuffer(&mDynVB);

    for (uint32 i = 0; i < sections.size(); ++i)
    {
        sections[i].drawSelf(ri);
    }
}

void Mesh::setLinkage( Entity* entity )
{

}
