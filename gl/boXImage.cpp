#define STB_IMAGE_IMPLEMENTATION
#include <miniBoxLog.h>
#include "stb_image.h"
#include "boXImage.h"

int boXImage::boXLoadImage(const char* filename)
{
	pixels = stbi_load(filename, &width, &height, &channels, 0);
	if (!pixels)
	{
		BOX_LOG_ERROR("Failed to load image: " << filename);
		return -1;
	}
	BOX_LOG_INFO("Loaded image: " << filename << " (" << width << "x" << height << ", " << channels << " channels)");
	return 0;

}

void boXImage::boXFreeImage()
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
