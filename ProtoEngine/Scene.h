#ifndef SCENE_H
#define SCENE_H

#include "typedefs.h"
#include "Entity.h"
#include <vector>

/*
a flattened scene structure
no spatial culling
*/
class Scene
{
public:
	Scene();
	~Scene();

	void add(Entity* pNode);
	void remove(Entity* pNode);

	std::vector<Entity*> mNodes;
};
#endif