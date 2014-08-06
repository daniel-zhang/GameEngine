#ifndef SCENE_H
#define SCENE_H

#include "typedefs.h"
#include "SceneNode.h"
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

	void add(SceneNode* pNode);
	void remove(SceneNode* pNode);

	std::vector<SceneNode*> mNodes;
};
#endif