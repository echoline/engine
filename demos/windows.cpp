#include <irrlicht.h>
#include "../EventReceiver.h"
#include "../TerrainSceneNode.h"
#include "../WindowSceneNode.h"
#include "../NineImageLoader.h"
using namespace irr;
using namespace scene;

#include <iostream>
using namespace std;

int main()
{
	// Event Receiver
	EventReceiver receiver;

	// start up the engine
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
		core::dimension2d<u32>(640,480), 32, true, false, false, 
		&receiver);

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* scenemgr = device->getSceneManager();

	// plan9 image format reader
	driver->addExternalImageLoader(new NineImageLoader(driver));

	// add a first person shooter style user controlled camera
	scene::ICameraSceneNode* camera = scenemgr->addCameraSceneNodeFPS();

	// add terrain scene node
	TerrainSceneNode* terrain = new TerrainSceneNode(NULL, scenemgr, -1);

	// create triangle selector for the terrain     
	scene::IMetaTriangleSelector* terrainSelector
		= terrain->createTriangleSelector(scenemgr, 0);
	terrain->setTriangleSelector(terrainSelector);

	scene::IMetaTriangleSelector *metaSelector = scenemgr->createMetaTriangleSelector();
	metaSelector->addTriangleSelector(terrainSelector);

	camera->setPosition(terrain->getTerrainCenter()
				+ core::vector3df(0, 400, 0));
	camera->setTarget(terrain->getTerrainCenter()
				+ core::vector3df(100, 400, 100));

	terrainSelector->drop();

	// create collision response animator and attach it to the camera
	metaSelector->addTriangleSelector(terrainSelector);
	scene::ISceneNodeAnimator* anim = scenemgr->createCollisionResponseAnimator(
				metaSelector, camera, 
				core::vector3df(20, 20, 20),
				core::vector3df(0,-1,0), // Gravity
				core::vector3df(0,0,0));
	camera->addAnimator(anim);
	anim->drop();

	// disable mouse cursor
	//device->getCursorControl()->setVisible(false);

	scene::ISceneCollisionManager* collMan = scenemgr->getSceneCollisionManager();
	u32 frames = 0;
	bool buttons[2] = {false, false};
	WindowSceneNode *w;
	WindowSceneNode *selwin = NULL;

	// draw everything
	while(device->run() && driver)
	{
		if (receiver.IsKeyDown(KEY_KEY_Q))
		{
			device->closeDevice();
			break;
		}

		terrain->shift(camera);

		core::stringw caption =(L"FPS: ");
		caption += driver->getFPS();
		device->setWindowCaption(caption.c_str());
		driver->beginScene(true, true, video::SColor(255,133,133,255));
		scenemgr->drawAll();

		if (receiver.IsLeftButtonDown())
		{
			// 500 unit line in direction of camera
			core::line3d<f32> ray;
       	        	ray.start = camera->getPosition();// + core::vector3df(0, -10, 0);
       	        	ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 500.0f;

			if (buttons[0] == false)
			{
				// ray/windows collision selection
				core::vector3df hitpoint;
				core::triangle3df hittri;
				w = (WindowSceneNode*)collMan->getSceneNodeAndCollisionPointFromRay(ray, hitpoint, hittri, 8);

				if (w != NULL)
				{
					w->select();
					selwin = w;
					w = NULL;
				}
				else
				{
					buttons[0] = true;
					w = new WindowSceneNode(NULL, scenemgr, 8, ray.end);
				}
			}
			else
			{
				w->setCorner(ray.end);
			}
		}
		else if (buttons[0] == true)
		{
			buttons[0] = false;

			w->made();
			w->drop();
			w = NULL;
		}

		if (receiver.IsRightButtonDown())
		{
			if (buttons[1] == false)
			{
				buttons[1] = true;
			}
		}
		else if (buttons[1] == true)
		{
			buttons[1] = false;
		}

		driver->endScene();

		video::IImage *image = driver->createImageFromFile("/home/eli/wsys/window");
		for (int i=0; i < WindowSceneNode::windows.size(); i++)
		{
			WindowSceneNode::windows[i]->update(image);
		}
		image->drop();
	}

	// delete device
	device->drop();
	return 0;
}
