#include "StdAfx.h"
#include "Scene.h"


CScene::CScene(void)
{
}


CScene::~CScene(void)
{
}

bool CScene::parse(std::string fname)
{
	ifstream file;
	file.open(fname.c_str(), ios::in);
	if (file.fail())
		return 1;
	string line;
	while (getline(file, line)) {
		istringstream iss(line);
		string type;
		iss >> type;
		if (type.compare("cam_width") == 0) {
			iss >> cam.mWidth;
		}
		if (type.compare("cam_height") == 0) {
			iss >> cam.mHeight;
		}
		if (type.compare("cam_FOV") == 0) {
			iss >> cam.FOV;
			cam.FOV = cam.FOV*3.141592f / 180;
		}
		if (type.compare("cam_eyep") == 0) {
			iss >> cam.position.x;
			iss >> cam.position.y;
			iss >> cam.position.z;
		}
		if (type.compare("cam_lookp") == 0) {
			iss >> cam.target.x;
			iss >> cam.target.y;
			iss >> cam.target.z;
		}
		if (type.compare("cam_up") == 0) {
			iss >> cam.up.x;
			iss >> cam.up.y;
			iss >> cam.up.z;
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
			iss >> sphere->absorption;
			iss >> sphere->refraction;
			mObjects.push_back(sphere);
		}
		if (type.compare("triangle") == 0) {
			CTriangle *triangle = new CTriangle;
			iss >> triangle->p0.x;
			iss >> triangle->p0.y;
			iss >> triangle->p0.z;
			iss >> triangle->p1.x;
			iss >> triangle->p1.y;
			iss >> triangle->p1.z;
			iss >> triangle->p2.x;
			iss >> triangle->p2.y;
			iss >> triangle->p2.z;
			iss >> triangle->reflect;
			iss >> triangle->amb.x;
			iss >> triangle->amb.y;
			iss >> triangle->amb.z;
			iss >> triangle->diff.x;
			iss >> triangle->diff.y;
			iss >> triangle->diff.z;
			iss >> triangle->spec.x;
			iss >> triangle->spec.y;
			iss >> triangle->spec.z;
			iss >> triangle->shininess;
			iss >> triangle->absorption;
			iss >> triangle->refraction;
			mObjects.push_back(triangle);
		}
		if (type.compare("light") == 0) {
			CLight *light = new CLight;
			iss >> light->pos.x;
			iss >> light->pos.y;
			iss >> light->pos.z;
			iss >> light->amb.x;
			iss >> light->amb.y;
			iss >> light->amb.z;
			iss >> light->diff.x;
			iss >> light->diff.y;
			iss >> light->diff.z;
			iss >> light->spec.x;
			iss >> light->spec.y;
			iss >> light->spec.z;
			mLights.push_back(light);
		}
	}
	return false;
}

