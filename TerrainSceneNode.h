#ifndef TERRAINSCENENODE_H
#define TERRAINSCENENODE_H

#include <irrlicht.h>
using namespace irr;

class TerrainSceneNode : public scene::ISceneNode
{
private:
	scene::ITerrainSceneNode *terrains[3][3];
	core::aabbox3d<float> Box;
	void centerOn(scene::ISceneNode*, int, int);

public:
	TerrainSceneNode(scene::ISceneNode*, scene::ISceneManager*, s32);
	~TerrainSceneNode();
	void render();
	const core::aabbox3d<float>& getBoundingBox() const;
	scene::IMetaTriangleSelector* createTriangleSelector(scene::ISceneManager*, s32);
	void shift(scene::ISceneNode*);

        const core::vector3df &getTerrainCenter() { 
		return terrains[1][1]->getTerrainCenter();
	}
};

#endif
