#ifndef WINDOWSCENENODE_H
#define WINDOWSCENENODE_H

#include <irrlicht.h>
#include <vector>
using namespace irr;

class WindowSceneNode : public scene::ISceneNode
{
	core::aabbox3d<f32> Box;
	scene::ISceneManager *smgr;
	video::SMaterial material;
	static std::vector<WindowSceneNode*> windows;
	core::vector3df corners[4];

public:
	WindowSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr,
			s32, core::aabbox3d<f32>, scene::ICameraSceneNode*);
	~WindowSceneNode();

	void deselectAll();
	void OnRegisterSceneNode();
	void render();
	const core::aabbox3df& getBoundingBox() const;
	bool selected;
};

#endif
