#include "NineImageLoader.h"
#include <sstream>
#include <iostream>
using namespace std;

NineImageLoader::NineImageLoader(video::IVideoDriver *d)
{
	driver = d;
}

bool NineImageLoader::isALoadableFileExtension(const io::path &filename) const
{
	return true;
}

bool NineImageLoader::header(char *data, unsigned int *w, unsigned int *h) const
{
	string str;
	unsigned int i = 0, x0, y0, x, y;
	str = string(data);
	delete data;			
	stringstream ss(str);

	while (ss >> str)
	{
		if (str == "x8r8g8b8")
			i = 1;
		else if (i == 1)
		{
			x0 = atoi(str.c_str());
			i++;
		}
		else if (i == 2)
		{
			y0 = atoi(str.c_str());
			i++;
		}
		else if (i == 3)
		{
			x = atoi(str.c_str());
			i++;
		}
		else if (i == 4)
		{
			y = atoi(str.c_str());
			i++;
		}
		else if (i == 5)
		{
			*h = (y-y0);
			*w = (x-x0);
			return true;
		}
		else
			i = 0;
	}

	return false;
}

bool NineImageLoader::isALoadableFileFormat(io::IReadFile *file) const
{
	unsigned int w, h;
	char* data = new char[1024];
	int r = file->read(data, 1024);
	data[r] = '\0';

	return header(data, &w, &h);
}

video::IImage* NineImageLoader::loadImage(io::IReadFile *file) const
{
	video::IImage* image = NULL;
	unsigned int w, h;
	char* data = new char[1024];
	int r = file->read(data, 1024);
	data[r] = '\0';

	if(header(data, &w, &h) == false)
		return NULL;

	data = new char[w*h*4+1024];
	r = file->read(data, w*h*4+1024);
	
	char* ptr = data;
	while ((*ptr < 127) && (*ptr >= ' '))
		ptr++;

//	cout << "read " << r << " bytes" << endl;

	image = driver->createImageFromData(video::ECF_A8R8G8B8, core::dimension2d<u32>(w, h), (void*)ptr);

	delete data;
	return image;
}
