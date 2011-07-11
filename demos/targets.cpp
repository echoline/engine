#include <irrlicht.h>
#include "../EventReceiver.h"
#include "../TerrainSceneNode.h"
using namespace irr;
using namespace scene;

// called when a mek scenenode is finished dying
class MekDieCallBack : public IAnimationEndCallBack
{
public:
	void OnAnimationEnd(IAnimatedMeshSceneNode *node)
	{
		node->setID(1 << 2);
		node->setMD2Animation(EMAT_BOOM);
	}
};

int main()
{
	// Event Receiver
	EventReceiver receiver;

	// see above
	MekDieCallBack callback;

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

	for (int i = 0; i < 10; i++)
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
	}
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
			

                	// Tracks the current intersection point with the level or a mesh
                	core::vector3df intersection;
                	// Used to show with triangle has been hit
                	core::triangle3df hitTriangle;

                	// This call is all you need to perform ray/triangle collision on every scene node
                	// that has a triangle selector, including the Quake level mesh.  It finds the nearest
                	// collision point/triangle, and returns the scene node containing that point.
                	// Irrlicht provides other types of selection, including ray/triangle selector,
                	// ray/box and ellipse/triangle selector, plus associated helpers.
                	// See the methods of ISceneCollisionManager
                	IAnimatedMeshSceneNode *selectedSceneNode =
                	        (scene::IAnimatedMeshSceneNode*)collMan->getSceneNodeAndCollisionPointFromRay(
                	                        ray,
                	                        intersection, // This will be the position of the collision
                	                        hitTriangle, // This will be the triangle hit in the collision
                	                        (1 << 0), // This ensures that only nodes that we have
                	                           // set up to be pickable are considered
                	                        0); // Check the entire scene (this is actually the implicit default)

			if(selectedSceneNode)
                	{
				// We need to reset the transform before doing our own rendering.
                	        //driver->setTransform(video::ETS_WORLD, core::matrix4());
                	        //driver->draw3DTriangle(hitTriangle, video::SColor(255,255,0,0));

				selectedSceneNode->setID(1 << 1);
				selectedSceneNode->setMD2Animation(scene::EMAT_DEATH_FALLBACKSLOW);
				selectedSceneNode->setLoopMode(false);
				selectedSceneNode->setAnimationEndCallback(&callback);
			}
		}
		else if (receiver.IsRightButtonDown())
		{
			// 1000 unit line in direction of camera
			core::line3d<f32> ray;
        	        ray.start = camera->getPosition() + core::vector3df(0, -10, 0);
        	        ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;

			video::SMaterial material;
			material.setFlag(video::EMF_LIGHTING, false);
			driver->setMaterial(material);
			driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
			driver->draw3DLine(ray.start, ray.end, video::SColor(128,0,255,0));

                	// Tracks the current intersection point with the level or a mesh
                	core::vector3df intersection;
                	// Used to show with triangle has been hit
                	core::triangle3df hitTriangle;

                	// This call is all you need to perform ray/triangle collision on every scene node
                	// that has a triangle selector, including the Quake level mesh.  It finds the nearest
                	// collision point/triangle, and returns the scene node containing that point.
                	// Irrlicht provides other types of selection, including ray/triangle selector,
                	// ray/box and ellipse/triangle selector, plus associated helpers.
                	// See the methods of ISceneCollisionManager
                	scene::IAnimatedMeshSceneNode *selectedSceneNode =
                	        (scene::IAnimatedMeshSceneNode*)collMan->getSceneNodeAndCollisionPointFromRay(
                	                        ray,
                	                        intersection, // This will be the position of the collision
                	                        hitTriangle, // This will be the triangle hit in the collision
                	                        (1 << 2), // This ensures that only nodes that we have
                	                           // set up to be pickable are considered
                	                        0); // Check the entire scene (this is actually the implicit default)

			if(selectedSceneNode)
                	{
				// We need to reset the transform before doing our own rendering.
                	        //driver->setTransform(video::ETS_WORLD, core::matrix4());
                	        //driver->draw3DTriangle(hitTriangle, video::SColor(255,255,0,0));
				selectedSceneNode->setID(1 << 0);
				selectedSceneNode->setMD2Animation(scene::EMAT_STAND);
				selectedSceneNode->setLoopMode(true);
				selectedSceneNode->setAnimationEndCallback(0);
			}
		}

		driver->endScene();
	}

	// delete device
	device->drop();
	return 0;
}
