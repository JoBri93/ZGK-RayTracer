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
using namespace std;

int main(int argc, char** argv)
{
	CScene scene;
	scene.parse("../scene_final.txt");

	CBitmap img;
	img.init(scene.cam.mWidth, scene.cam.mHeight);

	run(&scene, img);

	img.save("../output.bmp");

	return 0;
}

vec3 clamping(vec3 i)
{
	if (i.x > 1.0f) i.x = 1.0f;
	if (i.y > 1.0f) i.y = 1.0f;
	if (i.z > 1.0f) i.z = 1.0f;

	if (i.x < 0.0f) i.x = 0.0f;
	if (i.y < 0.0f) i.y = 0.0f;
	if (i.z < 0.0f) i.z = 0.0f;

	return i;
}

// G³ówna pêtla ray tracer'a
int run( CScene* scene, CBitmap& img ) {
	for( int y = 0; y < scene->cam.mHeight; y++ ) {
		for( int x = 0; x < scene->cam.mWidth; x++ ) {
			
			Output res;
			res.energy = 1.0f;
			res.color[0] = 0.0f;
			res.color[1] = 0.0f;
			res.color[2] = 0.0f;

			//promieñ pierwotny
			CRay primary_ray;
			primary_ray.pos = scene->cam.position;
			
			vec3 direction;
			direction = normalize(scene->cam.target - scene->cam.position);
			vec3 u = normalize(cross(scene->cam.up, direction));
			vec3 v = normalize(cross(u, direction));
			vec3 o = direction * (scene->cam.mWidth / (2 * tanf(scene->cam.FOV*0.5f))) - scene->cam.mWidth * 0.5f * u - scene->cam.mHeight *0.5f * v;

			mat3 dir_matrix(u.x, u.y, u.z, v.x, v.y, v.z, o.x, o.y, o.z);
			vec3 dir_vec(x, y, 1.0f);

			primary_ray.dir = dir_matrix * dir_vec;

			rayTrace(primary_ray, scene, &res);

			img.setPixel(x, scene->cam.mHeight-1-y, vec3(res.color[0], res.color[1], res.color[2]));
        }
	}
	return 0;
}

CSceneObject *findIntersection(CRay &ray, CScene* scene, bool closest_intersection)
{
	CSceneObject *intersection = NULL;
	float t = FLT_MAX;
	for (int i = 0; i < scene->mObjects.size(); i++)
	{
		if (scene->mObjects[i]->intersect(&ray) != -1) //&& scene->mObjects[i]->intersect(&ray) < t)
		{
			if (closest_intersection == true)
			{
				if (scene->mObjects[i]->intersect(&ray) < t)
				{
					t = scene->mObjects[i]->intersect(&ray);
					intersection = scene->mObjects[i];
				}
			}
			else
			{
				intersection = scene->mObjects[i];
				break;
			}
			//t = scene->mObjects[i]->intersect(&ray);
			//intersection = scene->mObjects[i];
		}
	}

	return intersection;
}

// Sledzenie pojedynczego promienia
int rayTrace(CRay &ray, CScene* scene, Output* res)
{
	CSceneObject *intersection = findIntersection(ray, scene, true);

	if (intersection != NULL)
	{
		vec3 i_out;
		i_out = intersection->amb*scene->mLights[0]->amb;
		for (int j = 0; j < scene->mLights.size(); j++)
		{
			//promieñ oœwietlenia
			CRay shadow_ray;
			float t = intersection->intersect(&ray);
			shadow_ray.pos = ray.pos + t * ray.dir;
			shadow_ray.dir = normalize(scene->mLights[j]->pos - shadow_ray.pos);

			CSceneObject *shadow = findIntersection(shadow_ray, scene, false);
			if (shadow == NULL)
			{
				vec3 i_dif, i_spe, obj_norm, h;

				h = normalize(shadow_ray.dir + normalize(-ray.dir));
				obj_norm = intersection->objectNorm(&ray);

				i_dif = intersection->diff * scene->mLights[j]->diff * dot(shadow_ray.dir, obj_norm);
				i_spe = intersection->spec * scene->mLights[j]->spec * pow(dot(obj_norm, h), intersection->shininess);

				i_dif = clamping(i_dif);
				i_spe = clamping(i_spe);

				i_out = i_out + i_dif + i_spe;

			}

			i_out = clamping(i_out);

			res->color[0] = i_out.x;
			res->color[1] = i_out.y;
			res->color[2] = i_out.z;

		}
		return 0;
	}
}