#ifndef NINEIMAGELOADER_H
#define NINEIMAGELOADER_H
#include <irrlicht.h>
using namespace irr;

class NineImageLoader : public video::IImageLoader
{
	bool header(char*, unsigned int*, unsigned int*) const;
	video::IVideoDriver *driver;

public:
	bool isALoadableFileExtension(const io::path &filename) const;
	bool isALoadableFileFormat(io::IReadFile *file) const;
	video::IImage* loadImage(io::IReadFile *file) const;

	NineImageLoader(video::IVideoDriver*);
};

#endif
