#ifndef TEXTUREBMP_H
#define TEXTUREBMP_H

#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
using namespace std;

class TextureBMP
{
    private:
        int imageWid, imageHgt, imageChnls;  //Width, height, number of channels
        char* imageData;
        bool loadBMPImage(char* string);
    public:
		TextureBMP(): imageWid(0), imageHgt(0), imageChnls(0) {}
        TextureBMP(char* string);
        glm::vec3 getColorAt(float s, float t);
};

#endif
