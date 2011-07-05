#include <irrlicht.h>
#include "../EventReceiver.h"
using namespace irr;

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

	// add a first person shooter style user controlled camera
	scene::ICameraSceneNode* camera = scenemgr->addCameraSceneNodeFPS();

	// add terrain scene node
	scene::ITerrainSceneNode* terrain = scenemgr->addTerrainSceneNode(
	    "../data/heightmap.bmp",
	    0,						  // parent node
	    -1,						 // node id
	    core::vector3df(0.f, 0.f, 0.f),	   // position
	    core::vector3df(0.f, 0.f, 0.f),	   // rotation
	    core::vector3df(40.f, 4.4f, 40.f),	// scale
	    video::SColor ( 255, 255, 255, 255 ),   // vertexColor
	    5,						  // maxLOD
	    scene::ETPS_17,				 // patchSize
	    4						   // smoothFactor
	    );

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

	terrain->setMaterialTexture(0, driver->getTexture("../data/texture.bmp"));
	  
	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	// create triangle selector for the terrain     
	scene::ITriangleSelector* terrainSelector
		= scenemgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(terrainSelector);

	scene::IMetaTriangleSelector *metaSelector = scenemgr->createMetaTriangleSelector();
	metaSelector->addTriangleSelector(terrainSelector);

	camera->setPosition(terrain->getTerrainCenter()
				+ core::vector3df(0, 400, 0));

	for (int i = 0; i < 15; i++) {
		// load and show quake2 .md2 model
		scene::IAnimatedMeshSceneNode* node = scenemgr->addAnimatedMeshSceneNode(
			scenemgr->getMesh("../data/mek/tris.md2"));

		// if everything worked, add a texture and disable lighting
		if (node)
		{
			node->setMaterialTexture(0, driver->getTexture("../data/mek/mek.pcx"));
			node->setMaterialFlag(video::EMF_LIGHTING, false);
			node->setPosition(terrain->getTerrainCenter()
				+ core::vector3df(rand() % 1000 - 500, 400, 
				  rand() % 1000 - 500));
			node->setMD2Animation(scene::EMAT_STAND);

			scene::ISceneNodeAnimator* anim = scenemgr->createCollisionResponseAnimator(
				    terrainSelector, node, core::vector3df(30, 40, 20),
				    core::vector3df(0,-10,0), // Gravity
				    core::vector3df(0,0,0));
			node->addAnimator(anim);
			anim->drop();

			scene::ITriangleSelector* s = scenemgr->createTriangleSelector(node);
			node->setTriangleSelector(s);

			metaSelector->addTriangleSelector(s);
			s->drop();

		}
	}
	terrainSelector->drop();

	// create collision response animator and attach it to the camera
	metaSelector->addTriangleSelector(terrainSelector);
	scene::ISceneNodeAnimator* anim = scenemgr->createCollisionResponseAnimator(
				metaSelector, camera, 
				core::vector3df(30, 40, 20),
				core::vector3df(0,0,0), // Gravity
				core::vector3df(0,0,0));
	camera->addAnimator(anim);
	anim->drop();


	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	u32 frames = 0;
	// draw everything
	while(device->run() && driver)
	{
		if (receiver.IsKeyDown(KEY_KEY_Q)) {
			device->closeDevice();
			break;
		}

		core::stringw caption =(L"FPS: ");
		caption += driver->getFPS();
		device->setWindowCaption(caption.c_str());
		driver->beginScene(true, true, video::SColor(255,155,155,255));
		scenemgr->drawAll();
		driver->endScene();
	}

	// delete device
	device->drop();
	return 0;
}
