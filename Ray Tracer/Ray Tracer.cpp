// Ray Tracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <glm\glm.hpp>
#include "FreeImage.h"

struct Output {
	float energy;
	int tree;
	float color[3];
};

int run( CScene* scene, CBitmap& img );
int rayTrace( CRay &ray, CScene* scene, Output* res );

using namespace glm;

int main(int argc, char** argv)
{
	//wczytywanie sceny

	CScene scene;
	scene.cam.mWidth = 1000;
	scene.cam.mHeight = 1000;

	CBitmap img;
	img.init(scene.cam.mWidth, scene.cam.mHeight);

	run(&scene, img);

	vec3 v1 = vec3(20, 30, 40);
	vec3 v2 = vec3(50, 60, 70);
	vec3 v3 = glm::cross(v1, v2); //iloczyn wektorowy

	printf("%f %f %f\n", v3.x, v3.y, v3.z);

	img.save("../output.bmp");

	return 0;
}

// G³ówna pêtla ray tracer'a
int run( CScene* scene, CBitmap& img ) {
		
	for( int y = 0; y < scene->cam.mHeight; y++ ) {
		for( int x = 0; x < scene->cam.mWidth; x++ ) {
			
			vec3 color;
			color.x = abs(cos(float(x) / scene->cam.mWidth * 20));
			color.y = abs(sin(float(y) / scene->cam.mHeight * 20));
			color.z = float(y) / scene->cam.mWidth;

			img.setPixel(x, y, color);
            // rayTrace(...) 
        }
	}
			
	return 0;
}

// Sledzenie pojedynczego promienia
int rayTrace( CRay &ray, CScene* scene, Output* res ) 
{
	return 0;
}