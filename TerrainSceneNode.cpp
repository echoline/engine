#include "TerrainSceneNode.h"
#include <iostream>
using namespace std;

TerrainSceneNode::TerrainSceneNode(scene::ISceneNode *parent, scene::ISceneManager *scenemgr, s32 id) : ISceneNode(parent, scenemgr, id)
{
	Box.reset(0, 0, 0);
	setScale(core::vector3df(40.f, 0.f, 40.f));

	for (int i=0; i < 3; i++) for (int j=0; j < 3; j++) {
		core::vector3df position(RelativeTranslation);
		position.X += (float)(i-1) * 128;
		position.Z += (float)(j-1) * 128;
		position *= RelativeScale;

		terrains[i][j] = scenemgr->addTerrainSceneNode("../data/heightmap.bmp", this,
			-1, position,
			RelativeRotation,	   // rotation
	   		RelativeScale,	// scale
	   		video::SColor ( 255, 255, 255, 255 ),   // vertexColor
	   		5,						  // maxLOD
	   		scene::ETPS_17,				 // patchSize
	   		4);						   // smoothFactor

		terrains[i][j]->setMaterialFlag(video::EMF_LIGHTING, false);
		terrains[i][j]->setMaterialTexture(0, scenemgr->getVideoDriver()
				->getTexture("../data/texture.bmp"));
		terrains[i][j]->setMaterialType(video::EMT_DETAIL_MAP);

		Box.addInternalBox(terrains[i][j]->getBoundingBox());
	}

	scenemgr->getRootSceneNode()->addChild(this);
}

TerrainSceneNode::~TerrainSceneNode()
{
	for (int i=0; i < 3; i++) for (int j=0; j < 3; j++)
		free(terrains[i][j]);
}

void TerrainSceneNode::render()
{
	for (int i=0; i < 3; i++) for (int j=0; j < 3; j++)
		terrains[i][j]->render();
}

const core::aabbox3d<float>& TerrainSceneNode::getBoundingBox() const
{
	return Box;
}	

scene::IMetaTriangleSelector* TerrainSceneNode::createTriangleSelector(scene::ISceneManager *scenemgr, s32 LOD)
{
	scene::IMetaTriangleSelector *ret = scenemgr->createMetaTriangleSelector();

	for (int i=0; i < 3; i++) for (int j=0; j < 3; j++)
		ret->addTriangleSelector(scenemgr->createTerrainTriangleSelector(terrains[i][j], LOD));

	return ret;
}

bool TerrainSceneNode::centerOn(int x, int y) {
	scene::ITerrainSceneNode *oterrains[3][3];

	if ((x == 1) && (y == 1))
		return false;

	for (int i=0; i < 3; i++) for (int j=0; j < 3; j++)
		oterrains[i][j] = terrains[i][j];

	switch(x) {
	case 0:
		for (int i=0; i < 2; i++) {
			core::vector3df position(oterrains[2][i]->getPosition());
			position.X -= 3 * 128 * RelativeScale.X;
			oterrains[2][i]->setPosition(position);
			terrains[0][i] = oterrains[2][i];
			terrains[1][i] = oterrains[0][i];
			terrains[2][i] = oterrains[1][i];
		}
		break;
	case 2:
		for (int i=0; i < 2; i++) {
			core::vector3df position(oterrains[0][i]->getPosition());
			position.X += 3 * 128 * RelativeScale.X;
			oterrains[0][i]->setPosition(position);
			terrains[2][i] = oterrains[0][i];
			terrains[1][i] = oterrains[2][i];
			terrains[0][i] = oterrains[1][i];
		}
		break;
	}

	switch(y) {
	case 0:
		for (int i=0; i < 2; i++) {
			core::vector3df position(oterrains[i][2]->getPosition());
			position.Z -= 3 * 128 * RelativeScale.Z;
			oterrains[i][2]->setPosition(position);
			terrains[i][0] = oterrains[i][2];
			terrains[i][1] = oterrains[i][0];
			terrains[i][2] = oterrains[i][1];
		}
		break;
	case 2:
		for (int i=0; i < 2; i++) {
			core::vector3df position(oterrains[i][0]->getPosition());
			position.X += 3 * 128 * RelativeScale.X;
			oterrains[i][0]->setPosition(position);
			terrains[i][2] = oterrains[i][0];
			terrains[i][1] = oterrains[i][2];
			terrains[i][0] = oterrains[i][1];
		}
		break;
	}

	return true;
}

bool TerrainSceneNode::shift(const core::vector3df &center)
{
	float miny = terrains[1][1]->getBoundingBox().MinEdge.Y;
	float maxy = terrains[1][1]->getBoundingBox().MaxEdge.Y;
	float avgy = miny + (maxy - miny) / 2.f;
	core::vector3df point = center;
	point.Y = avgy;

	int i = 0;
	for (; i < 9; i++)
	{
		if (terrains[i/3][i%3]->getBoundingBox().isPointInside(point))
		{
			break;
		}
	}

	int j = i%3;
	i /= 3;

	// not intersecting with any terrain(?)
	if (i == 3)
		return false;

	return centerOn(i, j);
}
