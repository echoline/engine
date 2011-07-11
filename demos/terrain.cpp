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
		core::dimension2d<u32>(640,480), 16, true, false, false, &receiver);

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* scenemgr = device->getSceneManager();

	// add a first person shooter style user controlled camera
	scene::ICameraSceneNode* camera = scenemgr->addCameraSceneNodeFPS();

	// add terrain scene node
	TerrainSceneNode* terrain = new TerrainSceneNode(NULL, scenemgr, -1);
	/*scenemgr->addTerrainSceneNode(
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
	*/

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

		if(terrain->shift(camera->getPosition()))
		{
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
		}
	}

	// delete device
	device->drop();
	return 0;
}

/*video::ITexture *generateTerrainTexture(video::IVideoDriver *driver, scene::ITerrainSceneNode *terrain)
{
	// Create the texture we'll use on the terrain
	// NOTE: The texture is 128 by 128 while the terrain is 129 by 129.  There are a couple of reasons
	// for this.  First it's better for texture sizes to be a power of 2.  Second this size lets us work
	// with the texture and terrain together as if each pair of triangles on the terrain is one pixel on
	// the texture.  It also gives us a bit of room when calculating slopes.
	const io::path path = "texture";
	video::ITexture *texture = driver->addTexture(core::dimension2d<u32>(128,128), path, video::ECF_A8R8G8B8);

	// Check if the colour format is what we expect.  IVideoDriver::addTexture() does not have to
	// use the colour format you specify so it must be checked.  Obviously it would be better to
	// generate a texture for all the colour formats but for this example I wanted to keep the whole
	// process simple.
	if (texture->getColorFormat() != video::ECF_A8R8G8B8)
		return NULL;

	// Lock the texture we created earlier so we can work on the data
	u32 *texData = static_cast<u32 *>(texture->lock());

	// Base color of black
	u32 color = 0xff000000;

	// For each point on the texture
	for (int x = 0; x < 128; x++)
	{
		for (int y = 0; y < 128; y++)
		{
			// NOTE: X and Y for the texture are X and Z respectively for the terrain.  I've tried to
			// keep my variable naming consistant based on whether the variable is referring more to the
			// terrain scene node or the texture.  But most things refer to both so Y and Z are used
			// somewhat interchangably.

			// Grab the scale of the terrain
			f32 xScale = terrain->getScale().X;
			f32 yScale = terrain->getScale().Y;
			f32 zScale = terrain->getScale().Z;

			// Calculate the slopes in the x and y directions
			// Another option would be to find the normal vector for the area being worked on
			// and using the y component as the slope.  This would also let you skip the next step
			// as the y component of the normal would always be positive.
			f32 xSlope = terrain->getHeight(x*xScale, y*zScale) - terrain->getHeight(x*xScale + 1.0f, y*zScale);
			f32 ySlope = terrain->getHeight(x*xScale, y*zScale) - terrain->getHeight(x*xScale, y*zScale + 1.0f);
			
			// Calculate the absolute values of the slopes
			xSlope = abs(xSlope);
			ySlope = abs(ySlope);

			// Calculate the red color based on slope
			// Steeper = more red
			s16 red = static_cast<s16>((xSlope > ySlope ? xSlope : ySlope) * 128);

			// Calculate the green value based on height
			// Lower = more green
			s16 green = 255 - static_cast<u8>(terrain->getHeight(x*xScale, y*zScale) / yScale);

			// Just set the blue
			s16 blue = 0x00;

			// NOTE: You can change the way the colours are calculated to adjust the results
			// but you should make sure to leave in the clamping I have applied so the colour
			// compilation below will work.
			//clampColours(red, green, blue);

			// Build the colour and set it in the correct place
			texData[x+128*y] = color + (red << 16) + (green << 8) + blue;
		}
	}

	// Unlock the texture so it can be used
	texture->unlock();

	// Return our pointer to the newely created texture.
	return texture;
}


*/
