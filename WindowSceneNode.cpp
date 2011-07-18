#include "WindowSceneNode.h"
#include <iostream>
using namespace std;

WindowSceneNode::WindowSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, core::vector3df cin) 
	: scene::ISceneNode(parent, mgr, id)
{
	video::S3DVertex vertices[4];
	vertices[0].Pos = vertices[1].Pos = vertices[2].Pos = vertices[3].Pos = cin;
	vertices[0].Color = vertices[1].Color = vertices[2].Color = vertices[3].Color = video::SColor(0xFFFFFFFF);

	for (int i=0; i < 2; i++) for (int j=0; j < 2; j++)
		vertices[i*2+j].TCoords = core::vector2df(j, i);

	smgr = mgr;

	material.Lighting = false;
	material.Wireframe = false;
	material.BackfaceCulling = false;
	material.FrontfaceCulling = false;
	material.AntiAliasing = video::EAAM_QUALITY;

	selected = false;
	creating = true;

	box = core::aabbox3df(vertices[0].Pos, vertices[3].Pos);

	float x = vertices[0].Pos.X - vertices[3].Pos.X;
	float y = abs(vertices[0].Pos.Y - vertices[3].Pos.Y);
	float z = vertices[0].Pos.Z - vertices[3].Pos.Z;
	x = sqrt(x * x + z * z);
	windim = core::dimension2df(x, y);

	mesh = new scene::SMesh();
	mesh->addMeshBuffer(new scene::SMeshBuffer());
	u16 indices[] = { 3,1,0, 0,2,3, };
	scene::IMeshBuffer *buf = mesh->getMeshBuffer(0);
	buf->append(vertices, 4, indices, 6);

	winstances++;
	texname = "window";
	texname += winstances;
	texture = smgr->getVideoDriver()->addTexture(core::dimension2d<u32>(256, 256), texname.c_str(), video::ECF_A8R8G8B8);
	cout << texname.c_str() << endl;

	u8 *data = (u8*)texture->lock();
	memset(data, 0xFF, texture->getPitch() * texture->getSize().Height);
	texture->unlock();

	material.setTexture(0, texture);

	smgr->getRootSceneNode()->addChild(this);
}

WindowSceneNode::~WindowSceneNode() {
	for (int i=0; i < windows.size(); i++)
	{
		if (windows[i] == this)
		{
			windows.erase(i);
		}
	}
}

void WindowSceneNode::select()
{
	for (int i=0; i < windows.size(); i++)
	{
		windows[i]->selected = false;
	}

	this->selected = true;
}

void WindowSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	scene::ISceneNode::OnRegisterSceneNode();
}

void WindowSceneNode::update(video::IImage *image)
{
	void *data = texture->lock();
	image->copyToScaling(data, 256, 256, video::ECF_A8R8G8B8, 256*4);
	texture->unlock();
}

void WindowSceneNode::render()
{
	video::S3DVertex *vertices = (video::S3DVertex*)mesh->getMeshBuffer(0)->getVertices();

	smgr->getVideoDriver()->setMaterial(material);
	smgr->getVideoDriver()->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	smgr->getVideoDriver()->drawMeshBuffer(mesh->getMeshBuffer(0));
}

const core::aabbox3df& WindowSceneNode::getBoundingBox() const
{
	return box;
}

void WindowSceneNode::setCorner(core::vector3df cin)
{
	video::S3DVertex *vertices = (video::S3DVertex*)mesh->getMeshBuffer(0)->getVertices();

	vertices[3].Pos = cin;
	vertices[1].Pos = vertices[3].Pos*core::vector3df(1,0,1) + vertices[0].Pos*core::vector3df(0,1,0);
	vertices[2].Pos = vertices[0].Pos*core::vector3df(1,0,1) + vertices[3].Pos*core::vector3df(0,1,0);

	box = core::aabbox3df(vertices[0].Pos, vertices[3].Pos);

	mesh->setDirty(scene::EBT_VERTEX);
}

void WindowSceneNode::made()
{
	video::S3DVertex *vertices = (video::S3DVertex*)mesh->getMeshBuffer(0)->getVertices();

	setTriangleSelector(smgr->createTriangleSelector(mesh, this));
	creating = false;

	float x = vertices[0].Pos.X - vertices[3].Pos.X;
	float y = abs(vertices[0].Pos.Y - vertices[3].Pos.Y);
	float z = vertices[0].Pos.Z - vertices[3].Pos.Z;
	x = sqrt(x * x + z * z);
	windim = core::dimension2di(x, y);

	windows.push_back(this);
}

core::array<WindowSceneNode*> WindowSceneNode::windows = core::array<WindowSceneNode*>();
u32 WindowSceneNode::winstances = 0;
