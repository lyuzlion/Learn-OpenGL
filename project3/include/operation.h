#ifndef OPERATION_H
#define OPERATION_H

#include "defs.h"
#include "Ray.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Sphere.h"
#include<Windows.h>
#include<GL/glut.h>
#include<cmath>
#include<iostream>
using namespace std;

float light_xx = 10;
float light_yy = -20;
float light_zz = -20;

#include "operation.h"


glm::vec3 proceduralTexture(Ray ray, float x, float y, float r, glm::vec3 col1, glm::vec3 col2, glm::vec3 col3)
{
	glm::vec3 col;
	if(ray.xpt.x < (x-r+(r/4)) || (ray.xpt.x >= (x-r+(3*r/4)) && ray.xpt.x < (x+r-(3*r/4))) || ray.xpt.x >= (x+r-(r/4))){
		if(ray.xpt.y < (y-r+(r/4)) || (ray.xpt.y >= (y-r+(3*r/4)) && ray.xpt.y < (y+r-(3*r/4))) || ray.xpt.y >= (y+r-(r/4))){
			return col1;
		} else {
			return col2;
		}
	} else {
		if(ray.xpt.y < (y-r+(r/4)) || (ray.xpt.y >= (y-r+(3*r/4)) && ray.xpt.y < (y+r-(3*r/4))) || ray.xpt.y >= (y+r-(r/4))){
			return col3;
		} else {
			return col1;
		}
	}
}



glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);  
	glm::vec3 light(light_xx, light_yy, light_zz); 
	float ambientTerm = 0.2;

	ray.closestPt(sceneObjects);	

	if(ray.xindex == -1) return backgroundCol;   

	glm::vec3 col = sceneObjects[ray.xindex]->getColor();

	glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);  
	glm::vec3 lightVector = light - ray.xpt; 
	glm::vec3 lightNormal = glm::normalize(lightVector);   
	float lDotn = glm::dot(lightNormal, normalVector);      

	glm::vec3 reflVector = glm::reflect(-lightVector, normalVector);
	glm::vec3 reflNormal = glm::normalize(reflVector);  
	float spec = glm::dot(reflNormal, -ray.dir);   

	if(spec < 0)
	{
		spec = 0;
	}else if(ray.xindex == 1){
		spec = glm::pow(spec, 40.0);
	}else{
		spec = glm::pow(spec, 7.0);
	}

	Ray shadow(ray.xpt, lightNormal);
	shadow.closestPt(sceneObjects);  

	float d = glm::distance(ray.xpt, light); 

	glm::vec3 colorSum;	


	if(ray.xindex == 1 && step<MAX_STEPS){ 
		float eta = 1 / 1.005;
		glm::vec3 g = glm::refract(ray.dir, normalVector, eta);
		Ray refrRay1(ray.xpt, g);
		refrRay1.closestPt(sceneObjects);
		glm::vec3 m = sceneObjects[refrRay1.xindex]->normal(refrRay1.xpt);
		glm::vec3 h = glm::refract(g, -m, 1.0f / eta);
		Ray refrRay2(refrRay1.xpt, h);

		glm::vec3 refractedCol = trace(refrRay2, step);//Recursion!

		colorSum = colorSum + (0.8f * refractedCol);
	}

	if((lDotn <=0)||((shadow.xindex > -1) && (shadow.xdist < d))){
		if(shadow.xindex == 1){
			return (col * 0.67f + colorSum);
		} else {
			return (col * ambientTerm + colorSum);
		}
	} else {
		return (ambientTerm * col + lDotn * col + spec + colorSum);
	}
}


void display()
{
	//grid point
	float xp, yp;
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height

	//The eye position (source of primary rays) is the origin
	glm::vec3 eye(0., 0., 0.);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//Each cell is a quad.
	glBegin(GL_QUADS);

	//For each grid point xp, yp
	for (register int i = 0; i < NUMDIV;++i)
	{
		xp = XMIN + i * cellX;
		for (register int j = 0; j < NUMDIV;++j)
		{
			yp = YMIN + j * cellY;

			//divided each square pixel into four to perform supersampling
			//将每个正方形像素分成4个进行超采样
			glm::vec3 uleft(xp+0.25*cellX, yp+0.75*cellY, -EDIST);	//direction of the primary ray
		    glm::vec3 uright(xp+0.75*cellX, yp+0.75*cellY, -EDIST);
		    glm::vec3 lleft(xp+0.25*cellX, yp+0.25*cellY, -EDIST);
		    glm::vec3 lright(xp+0.75*cellX, yp+0.25*cellY, -EDIST);

			//Create a ray originating from the camera in the direction 'dir'
			Ray ray1 = Ray(eye, uleft);
			ray1.normalize();
			Ray ray2 = Ray(eye, uright);
			ray2.normalize();
			Ray ray3 = Ray(eye, lleft);
			ray3.normalize();
			Ray ray4 = Ray(eye, lright);
			ray4.normalize();

			glm::vec3 col1 = trace (ray1, 1); //Trace the primary ray and get the colour value
		    glm::vec3 col2 = trace (ray2, 1);
		    glm::vec3 col3 = trace (ray3, 1);
		    glm::vec3 col4 = trace (ray4, 1);

			glm::vec3 col;    //finding tthe average colour of the four rays in the pixel
		    col.r = (col1.r + col2.r + col3.r + col4.r) / 4;
		    col.g = (col1.g + col2.g + col3.g + col4.g) / 4;
		    col.b = (col1.b + col2.b + col3.b + col4.b) / 4;

			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);


		}
	}

	glEnd();
    glFlush();
}




void createSpheres()
{
	Sphere *sphere1 = new Sphere(glm::vec3(-15, -7.0, -180.5), 4.0, glm::vec3(1, 0, 0));   // red Reflecting sphere
	Sphere *sphere2 = new Sphere(glm::vec3(-3, -11, -70.0), 0, glm::vec3(1, 0.6, 0));    // Refracted Sphere
	Sphere *sphere3 = new Sphere(glm::vec3(8.0, -15, -150), 0, glm::vec3(1, 0, 0));      // Textured Sphere
	Sphere *sphere4 = new Sphere(glm::vec3(-15, -15, -180.5), 4.0, glm::vec3(0, 0, 1));    // Green Sphere
	
	sceneObjects.push_back(sphere1); 
	sceneObjects.push_back(sphere2); 
	sceneObjects.push_back(sphere3);
	sceneObjects.push_back(sphere4);  
}

void createFloor()
{
	Plane *plane1 = new Plane (glm::vec3(-20., 40, -40),     //Point A
								glm::vec3(-20., -20, -40),    //Point B
								glm::vec3(-20., -20, -200),   //Point C
								glm::vec3(-20., 40, -200),  //Point D
								glm::vec3(1, 1, 1));     //Colour
	Plane *plane3 = new Plane (glm::vec3(20., 40, -200),     //Point A
								glm::vec3(-20., 40, -200),    //Point B
								glm::vec3(-20., -20, -200),   //Point C
								glm::vec3(20., -20, -200),  //Point D
								glm::vec3(1, 1, 1));     //Colour
	
	Plane *plane2 = new Plane (glm::vec3(20., 40, -40),     //Point A
								glm::vec3(20., -20, -40),    //Point B
								glm::vec3(20., -20, -200),   //Point C
								glm::vec3(20., 40, -200),  //Point D
								glm::vec3(1, 1, 1));     //Colour


	// 天花板
	Plane *plane5 = new Plane (glm::vec3(-20., 40, -40),     //Point A
								glm::vec3(-20., 40, -200),    //Point B
								glm::vec3(20., 40, -200),   //Point C
								glm::vec3(20., 40, -40),  //Point D
								glm::vec3(1, 1, 1));     //Colour
	
	//-- 地板
	Plane *plane4 = new Plane (glm::vec3(-20., -20, -40),     //Point A
								glm::vec3(20., -20, -40),    //Point B
								glm::vec3(20., -20, -200),   //Point C
								glm::vec3(-20., -20, -200),  //Point D
								glm::vec3(0.7, 0.4, 0.3));     //Co
	sceneObjects.push_back(plane1);
	sceneObjects.push_back(plane3);
	sceneObjects.push_back(plane2);
	sceneObjects.push_back(plane4);
	sceneObjects.push_back(plane5);

}

void createBox()
{

}


void initialize()
{
	glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(1, 1, 1, 1);
	createSpheres();
	createFloor();

	char text[] = "./src/Squares.bmp";
	char *s = text;
	texture = TextureBMP(s);
}

#endif

