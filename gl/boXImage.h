#pragma once

struct boXImg
{
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* pixels = nullptr;
};

class boXImage {
public:
   
	boXImage() = default;
	~boXImage() = default;

	bool boXLoadImage(const char* filename, boXImg& image);

    void boXFreeImage(boXImage& image);

private:
    int width = 0;
    int height = 0;
    int channels = 0;

    unsigned char* pixels = nullptr;

};

