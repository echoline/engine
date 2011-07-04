all: md2show terrain

md2show: md2show.cpp 
	g++ -o md2show md2show.cpp ../EventReceiver.cpp -I/usr/include/irrlicht/ -lIrrlicht

terrain: terrain.cpp 
	g++ -o terrain terrain.cpp ../EventReceiver.cpp -I/usr/include/irrlicht/ -lIrrlicht

clean:
	rm -f md2show terrain
