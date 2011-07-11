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
	   		video::SColor ( 255, 133, 133, 133 ),   // vertexColor
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

void TerrainSceneNode::centerOn(scene::ISceneNode *in, int x, int y) {
	if ((x == 1) && (y == 1))
		return;

	core::vector3df position(in->getPosition());
	position.Y = 10.f;

	cout << x << " " << y << endl;

	switch(x) {
	case 0:
		position.X += 128 * RelativeScale.X;
		break;
	case 2:
		position.X -= 128 * RelativeScale.X;
	default:
		break;
	}

	switch(y) {
	case 0:
		position.Z += 128 * RelativeScale.Z;
		break;
	case 2:
		position.Z -= 128 * RelativeScale.Z;
	default:
		break;
	}

	in->setPosition(position);
}

void TerrainSceneNode::shift(ISceneNode *in)
{
	float miny = terrains[1][1]->getBoundingBox().MinEdge.Y;
	float maxy = terrains[1][1]->getBoundingBox().MaxEdge.Y;
	float avgy = miny + (maxy - miny) / 2.f;
	core::vector3df point = in->getPosition();
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
		return;

	centerOn(in, i, j);
}

