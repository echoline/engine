#include <irrlicht.h>
#include "../EventReceiver.h"
#include "../TerrainSceneNode.h"
using namespace irr;
using namespace scene;

class Window : public ISceneNode
{
	core::aabbox3d<f32> Box;
	scene::ISceneManager *smgr;
	video::SMaterial material;

public:
	Window(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, core::aabbox3d<f32> b)
                : scene::ISceneNode(parent, mgr, id)
        {
		Box = b;
		smgr = mgr;
		material.Lighting = false;
		material.Wireframe = true;

		smgr->getRootSceneNode()->addChild(this);
	}

	void OnRegisterSceneNode()
        {
                if (IsVisible)
                        SceneManager->registerNodeForRendering(this);

                scene::ISceneNode::OnRegisterSceneNode();
        }

	void render()
	{
		smgr->getVideoDriver()->setMaterial(material);
		smgr->getVideoDriver()->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		smgr->getVideoDriver()->draw3DBox(Box, video::SColor(128,255,0,0));			
	}

	const core::aabbox3df& getBoundingBox() const
	{
		return Box;
	}
};

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
				+ core::vector3df(100, 100, 100));

/*	for (int i = 0; i < 10; i++)
	{
		// load and show quake2 .md2 model
		IAnimatedMeshSceneNode* node = scenemgr->addAnimatedMeshSceneNode(scenemgr->getMesh(
				"../data/mek/tris.md2"),
				0,
				(1 << 0));

		// if everything worked, add a texture and disable lighting
		if (node)
		{
			node->setMaterialTexture(0, driver->getTexture("../data/mek/mek.pcx"));
			node->setMaterialFlag(video::EMF_LIGHTING, false);
			node->setPosition(terrain->getTerrainCenter()
				+ core::vector3df(rand() % 1000 - 500, 400, 
				  rand() % 1000 - 500));
			node->setMD2Animation(scene::EMAT_STAND);

			const core::aabbox3d<f32>& box = node->getBoundingBox();
			core::vector3df radius = box.MaxEdge - box.getCenter();

			scene::ISceneNodeAnimator* anim = scenemgr->createCollisionResponseAnimator(
				    terrainSelector, node, 
				    radius,
				    core::vector3df(0,-10,0), // Gravity
				    core::vector3df(0,0,0));
			node->addAnimator(anim);
			anim->drop();

			scene::ITriangleSelector* s = scenemgr->createTriangleSelector(node);
			node->setTriangleSelector(s);

			metaSelector->addTriangleSelector(s);
			s->drop();

		}
	}*/
	terrainSelector->drop();

	// create collision response animator and attach it to the camera
	metaSelector->addTriangleSelector(terrainSelector);
	scene::ISceneNodeAnimator* anim = scenemgr->createCollisionResponseAnimator(
				metaSelector, camera, 
				core::vector3df(20, 20, 20),
				core::vector3df(0,-20,0), // Gravity
				core::vector3df(0,0,0));
	camera->addAnimator(anim);
	anim->drop();


	// TODO: crosshair
	// disable mouse cursor
	//device->getCursorControl()->setVisible(false);

	scene::ISceneCollisionManager* collMan = scenemgr->getSceneCollisionManager();
	u32 frames = 0;
	bool buttons[2] = {false, false};
	core::vector3df start;
	core::aabbox3d<f32> box;
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
		driver->beginScene(true, true, video::SColor(255,155,155,255));
		scenemgr->drawAll();

		if (receiver.IsLeftButtonDown())
		{
			// 1000 unit line in direction of camera
			core::line3d<f32> ray;
       	        	ray.start = camera->getPosition() + core::vector3df(0, -10, 0);
       	        	ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;

			video::SMaterial material;
			material.setFlag(video::EMF_LIGHTING, false);
			driver->setMaterial(material);
			driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
			driver->draw3DLine(ray.start, ray.end, video::SColor(128,255,0,0));

			if (buttons[0] == false)
			{
				buttons[0] = true;
				start = ray.end;
			}
			else
			{
				driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
				box = core::aabbox3d<f32>(start);
				box.addInternalPoint(ray.end);
				driver->draw3DBox(box, video::SColor(128,255,0,0));
			}
		}
		else if (buttons[0] == true)
		{
			buttons[0] = false;

			Window *w = new Window(NULL, scenemgr, -1, box);
			//w->drop();
			//w = NULL;
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
	}

	// delete device
	device->drop();
	return 0;
}
