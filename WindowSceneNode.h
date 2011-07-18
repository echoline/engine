#ifndef WINDOWSCENENODE_H
#define WINDOWSCENENODE_H

#include <irrlicht.h>
#include <vector>
using namespace irr;

class WindowSceneNode : public scene::ISceneNode
{
	scene::ISceneManager *smgr;
	video::SMaterial material;
	core::aabbox3d<f32> box;
	core::dimension2d<u32> windim;
	scene::SMesh *mesh;
	bool creating;
	bool selected;
	core::stringc texname;
	video::ITexture *texture;

public:
	WindowSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr,
			s32, core::vector3df);
	~WindowSceneNode();

	void select();
	void OnRegisterSceneNode();
	void render();
	const core::aabbox3df& getBoundingBox() const;
	void setCorner(core::vector3df);
	void made();
	void update(video::IImage*);
	
	static core::array<WindowSceneNode*> windows;
	static u32 winstances;
};

#endif
