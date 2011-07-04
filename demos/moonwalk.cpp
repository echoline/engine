#include <irrlicht.h>
#include "../EventReceiver.h"

int main()
{
	// Event Receiver
	EventReceiver receiver;

	// start up the engine
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
		core::dimension2d<u32>(640,480), 16, false, false, false, &receiver);

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* scenemgr = device->getSceneManager();

	// load and show quake2 .md2 model
	scene::IAnimatedMeshSceneNode* node = scenemgr->addAnimatedMeshSceneNode(
		scenemgr->getMesh("../data/mek/tris.md2"));

	// if everything worked, add a texture and disable lighting
	if (node)
	{
		node->setMaterialTexture(0, driver->getTexture("../data/mek/chief.pcx"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_RUN);

		scene::ISceneNodeAnimator* anim =
                        scenemgr->createFlyStraightAnimator(core::vector3df(200,0,0),
                        core::vector3df(-200,0,0), 9000, true);
                if (anim)
                {
                        node->addAnimator(anim);
                        anim->drop();
                }
	}

	// add a first person shooter style user controlled camera
	scene::ICameraSceneNode *camera = scenemgr->addCameraSceneNodeFPS();
	camera->setPosition(core::vector3df(0, 0, -100));

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	u32 frames = 0;
	// draw everything
	while(device->run() && driver)
	{
		if (receiver.IsKeyDown(KEY_KEY_Q))
			break;

		core::stringw caption =(L"FPS: ");
		caption += driver->getFPS();
		device->setWindowCaption(caption.c_str());
		driver->beginScene(true, true, video::SColor(255,113,113,113));
		scenemgr->drawAll();
		driver->endScene();
	}

	// delete device
	device->drop();
	return 0;
}
