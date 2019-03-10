// Ray Tracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <glm\glm.hpp>
#include "FreeImage.h"

#include <fstream>
#include <sstream>

struct Output {
	float energy;
	int tree;
	float color[3];
};

int run( CScene* scene, CBitmap& img );
int rayTrace( CRay &ray, CScene* scene, Output* res );

using namespace glm;
using namespace std;

int main(int argc, char** argv)
{
	//wczytywanie sceny
	CScene scene;

	ifstream file;
	string filename = "../scene_simple.txt";
	file.open(filename.c_str(), ios::in); 
	if (file.fail())
		return 1;
	string line;
	while (getline(file, line)) {
		istringstream iss(line);
		string type;
		iss >> type;
		if (type.compare("cam_width") == 0) {
			iss >> scene.cam.mWidth;
		}
		if (type.compare("cam_height") == 0) {
			iss >> scene.cam.mHeight;
		}
		if (type.compare("cam_FOV") == 0) {
			iss >> scene.cam.FOV;
		}
		if (type.compare("cam_eyep") == 0) {
			iss >> scene.cam.position.x;
			iss >> scene.cam.position.y;
			iss >> scene.cam.position.z;
		}
		if (type.compare("cam_lookp") == 0) {
			iss >> scene.cam.target.x;
			iss >> scene.cam.target.y;
			iss >> scene.cam.target.z;
		}
		if (type.compare("cam_up") == 0) {
			iss >> scene.cam.up.x;
			iss >> scene.cam.up.y;
			iss >> scene.cam.up.z;
		}
		if (type.compare("sphere") == 0) {
			CSphere *sphere = new CSphere;
			iss >> sphere->radius;
			iss >> sphere->center.x;
			iss >> sphere->center.y;
			iss >> sphere->center.z;
			iss >> sphere->reflect;
			iss >> sphere->amb.x;
			iss >> sphere->amb.y;
			iss >> sphere->amb.z;
			iss >> sphere->diff.x;
			iss >> sphere->diff.y;
			iss >> sphere->diff.z;
			iss >> sphere->spec.x;
			iss >> sphere->spec.y;
			iss >> sphere->spec.z;
			iss >> sphere->shininess;
			scene.mObjects.push_back(sphere);
		}
	}
	//scene.cam.mWidth = 1000;
	//scene.cam.mHeight = 1000;

	CBitmap img;
	img.init(scene.cam.mWidth, scene.cam.mHeight);

	run(&scene, img);

	img.save("../output.bmp");

	return 0;
}

// G³ówna pêtla ray tracer'a
int run( CScene* scene, CBitmap& img ) {
		
	for( int y = 0; y < scene->cam.mHeight; y++ ) {
		for( int x = 0; x < scene->cam.mWidth; x++ ) {
			
			//vec3 color;
			//color.x = abs(cos(float(x) / scene->cam.mWidth * 20));
			//color.y = abs(sin(float(y) / scene->cam.mHeight * 20));
			//color.z = float(y) / scene->cam.mWidth;

			//img.setPixel(x, y, color);

			Output res;
			res.energy = 1.0f;
			res.color[0] = 0.0f;
			res.color[1] = 0.0f;
			res.color[2] = 0.0f;

			//promieñ pierwotny
			CRay primary_ray;
			primary_ray.pos = scene->cam.position;
			
			vec3 direction;
			direction = scene->cam.target - scene->cam.position;
			vec3 u = cross(scene->cam.up, direction);
			vec3 v = cross(u, direction);
			vec3 o = direction * (scene->cam.mWidth / (2 * tanf(scene->cam.FOV*0.5f))) - scene->cam.mWidth * 0.5f * u - scene->cam.mHeight *0.5f * v;

			mat3 dir_matrix(u.x, u.y, u.z, v.x, v.y, v.z, o.x, o.y, o.z);
			//mat3 dir_matrix(u.x, v.x, o.x, u.y, v.y, o.y, u.z, v.z, o.z);
			vec3 dir_vec(x, y, 1.0f);

			primary_ray.dir = dir_matrix * dir_vec;

			rayTrace(primary_ray, scene, &res);

			img.setPixel(x, y, vec3(res.color[0],res.color[1],res.color[2]));
        }
	}
			
	return 0;
}

// Sledzenie pojedynczego promienia
int rayTrace( CRay &ray, CScene* scene, Output* res ) 
{
	for (int i = 0; i < scene->mObjects.size(); i++)
	{
		//musisz porównywaæ wartoœci t wszystkich obiektów
		if (scene->mObjects[i]->intersect(&ray) != -1)
		{
			res->color[0] = scene->mObjects[i]->diff.x;
			res->color[1] = scene->mObjects[i]->diff.y;
			res->color[2] = scene->mObjects[i]->diff.z;
		}
	}
	return 0;
}