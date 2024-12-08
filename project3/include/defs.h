#ifndef DEFS_H
#define DEFS_H
#include<vector>
#include "TextureBMP.h"
#include "SceneObject.h"
//global constants
extern const float WIDTH;  
extern const float HEIGHT;
extern const float EDIST;
extern const int NUMDIV;
extern const int MAX_STEPS;
extern const float XMIN;
extern const float XMAX;
extern const float YMIN;
extern const float YMAX;

//declaring the texture
extern TextureBMP texture;

//A global list containing pointers to objects in the scene
extern vector<SceneObject*> sceneObjects;
#endif