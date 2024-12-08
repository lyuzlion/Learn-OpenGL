#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H
#include "glm/glm.hpp"

class SceneObject
{
    protected:
        glm::vec3 color;
    public:
        SceneObject(){}
        virtual float intersect(glm::vec3 pos, glm::vec3 dir) = 0;
        virtual glm::vec3 normal(glm::vec3 pos) = 0;
        virtual ~SceneObject(){}
        glm::vec3 getColor();
        void setColor(glm::vec3 col);
};

#endif