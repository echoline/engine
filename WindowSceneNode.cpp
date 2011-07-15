#include "WindowSceneNode.h"

WindowSceneNode::WindowSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, core::aabbox3d<f32> b, scene::ICameraSceneNode *camera) 
	: scene::ISceneNode(parent, mgr, id)
{
	float cam[4];
	float min[4];
	float max[4];
	float len[3];
	float vals[3];

	Box = b;
	smgr = mgr;
	material.Lighting = false;
	material.Wireframe = false;
	selected = false;

	camera->getPosition().getAs4Values(cam);
	Box.MinEdge.getAs4Values(min);
	Box.MaxEdge.getAs4Values(max);

	smgr->getRootSceneNode()->addChild(this);

	for (int i=0; i < 3; i++) {
		len[i] = max[i] - min[i];

		if (abs(cam[i] - min[i]) > abs(cam[i] - max[i]))
			vals[i] = min[i];
		else
			vals[i] = max[i];
	}

	corners[0] = core::vector3df(vals[0], vals[1], vals[2]);

	scene::IMeshSceneNode *sphere = smgr->addSphereSceneNode(5.0f, 16, this, 16, corners[0]);
}

WindowSceneNode::~WindowSceneNode() {
	std::vector<WindowSceneNode*>::iterator it;

	for (it=windows.begin(); it < windows.end(); it++)
	{
		if (*it == this)
		{
			windows.erase(it);
		}
	}
}

void WindowSceneNode::deselectAll()
{
	std::vector<WindowSceneNode*>::iterator it;

	for (it=windows.begin(); it < windows.end(); it++)
	{
		(*it)->selected = false;
	}
	windows.push_back(this);
}

void WindowSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	scene::ISceneNode::OnRegisterSceneNode();
}

void WindowSceneNode::render()
{
	smgr->getVideoDriver()->setMaterial(material);
	smgr->getVideoDriver()->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	smgr->getVideoDriver()->draw3DBox(Box, selected?
					video::SColor(128, 0, 133, 255):
					video::SColor(128, 0, 255, 255));
}

const core::aabbox3df& WindowSceneNode::getBoundingBox() const
{
	return Box;
}

std::vector<WindowSceneNode*> WindowSceneNode::windows = std::vector<WindowSceneNode*>();

