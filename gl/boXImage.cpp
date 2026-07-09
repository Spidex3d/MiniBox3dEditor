#define STB_IMAGE_IMPLEMENTATION
#include <miniBoxLog.h>
#include "stb_image.h"
#include "boXImage.h"

bool boXImage::boXLoadImage(const char* filename, boXImg& image)
{
	image.pixels = stbi_load(filename, &image.width, &image.height,	&image.channels, 4); // force RGBA

	if (!image.pixels)
		return false;

	image.channels = 4;
	return true;
}

void boXImage::boXFreeImage(boXImage& image)
{
	if (pixels)
	{
		stbi_image_free(pixels);
		pixels = nullptr;
	}

	width = 0;
	height = 0;
	channels = 0;
}
