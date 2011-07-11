#include <irrlicht.h>
#include "../EventReceiver.h"
#include "../TerrainSceneNode.h"
using namespace irr;

int main()
{
	// Event Receiver
	EventReceiver receiver;

	// start up the engine
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
		core::dimension2d<u32>(640,480), 16, false, false, false, &receiver);

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* scenemgr = device->getSceneManager();

	// add a first person shooter style user controlled camera
	scene::ICameraSceneNode* camera = scenemgr->addCameraSceneNodeFPS();

	// add terrain scene node
	TerrainSceneNode* terrain = new TerrainSceneNode(NULL, scenemgr, -1);

	// create triangle selector for the terrain     
	scene::IMetaTriangleSelector* selector
		= terrain->createTriangleSelector(scenemgr, 0);
	terrain->setTriangleSelector(selector);

	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = scenemgr->createCollisionResponseAnimator(
		    selector, camera, core::vector3df(60,100,60),
		    core::vector3df(0,-1,0), // Gravity
		    core::vector3df(0,0,0));
	selector->drop();
	camera->addAnimator(anim);
	anim->drop();

	camera->setPosition(terrain->getTerrainCenter() + core::vector3df(0, 400, 0));

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
		driver->beginScene(true, true, video::SColor(255,133,133,255));
		scenemgr->drawAll();
		driver->endScene();

		terrain->shift(camera);
	}

	// delete device
	device->drop();
	return 0;
}

