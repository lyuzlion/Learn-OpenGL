#include "Ray.h"

void Ray::normalize()
{
    dir = glm::normalize(dir);
}

void Ray::closestPt(std::vector<SceneObject*> &sceneObjects)
{
    glm::vec3 point(0, 0, 0);
    float min = 1.e+6;
    for (int i = 0; i < (int)sceneObjects.size();++i)
    {
        float t = sceneObjects[i]->intersect(pt, dir);
        if(t>0)//Intersects the object/ /相交的对象
        {
            point = pt + dir * t;
            if(t<min)
            {
                this->xpt = point;
                this->xindex = i;
                this->xdist = t;
                min = t;
            }
        }
    }
}