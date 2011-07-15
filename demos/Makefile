all: md2show terrain moonwalk targets windows

moonwalk: moonwalk.cpp 
	g++ -o moonwalk moonwalk.cpp ../EventReceiver.cpp -I/usr/include/irrlicht/ -lIrrlicht

md2show: md2show.cpp 
	g++ -o md2show md2show.cpp ../EventReceiver.cpp -I/usr/include/irrlicht/ -lIrrlicht

terrain: terrain.cpp 
	g++ -o terrain terrain.cpp ../EventReceiver.cpp ../TerrainSceneNode.cpp -I/usr/include/irrlicht/ -lIrrlicht

targets: targets.cpp 
	g++ -o targets targets.cpp ../EventReceiver.cpp ../TerrainSceneNode.cpp -I/usr/include/irrlicht/ -lIrrlicht

windows: windows.cpp 
	g++ -o windows windows.cpp ../EventReceiver.cpp ../TerrainSceneNode.cpp ../WindowSceneNode.cpp -I/usr/include/irrlicht/ -lIrrlicht

clean:
	rm -f md2show terrain moonwalk targets windows
