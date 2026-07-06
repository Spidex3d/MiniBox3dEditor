#pragma once

class boXImage {
public:
   
	boXImage() = default;
	~boXImage() = default;

	int boXLoadImage(const char* filename);

    void boXFreeImage();

private:
    int width = 0;
    int height = 0;
    int channels = 0;

    unsigned char* pixels = nullptr;

};

