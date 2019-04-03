// Ray Tracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <glm\glm.hpp>
#include "FreeImage.h"

using namespace glm;
using namespace std;

struct Output {
	float energy;
	int tree;
	float color[3];
};

int run( CScene* scene, CBitmap& img );
int rayTrace( CRay &ray, CScene* scene, Output* res);
vec3 primaryRayDir(CRay ray, CScene *scene, float x, float y);

int main(int argc, char** argv)
{
	CScene scene;
	scene.parse("../scene.txt");
	
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

vec3 primaryRayDir(CRay ray, CScene *scene, float x, float y)
{
	vec3 direction;

	direction = normalize(scene->cam.target - scene->cam.position);
	vec3 u = normalize(cross(scene->cam.up, direction));
	vec3 v = normalize(cross(u, direction));
	vec3 o = direction * (scene->cam.mWidth / (2 * tanf(scene->cam.FOV*0.5f))) - scene->cam.mWidth * 0.5f * u - scene->cam.mHeight *0.5f * v;

	mat3 dir_matrix(u.x, u.y, u.z, v.x, v.y, v.z, o.x, o.y, o.z);
	vec3 dir_vec(x, y, 1.0f);

	return normalize(dir_matrix * dir_vec);
}

// G³ówna pêtla ray tracer'a
int run( CScene* scene, CBitmap& img ) {
	for( int y = 0; y < scene->cam.mHeight; y++ ) {
		for( int x = 0; x < scene->cam.mWidth; x++ ) {

			//promieñ pierwotny
			CRay primary_ray;
			primary_ray.pos = scene->cam.position;

			vec3 pixelColor = vec3(0);
			for (int aaX=0; aaX<3; aaX++)
			{
				for (int aaY=0; aaY<3; aaY++)
				{
					Output res;
					res.energy = 1.0f;
					res.color[0] = 0.0f;
					res.color[1] = 0.0f;
					res.color[2] = 0.0f;

					primary_ray.dir = primaryRayDir(primary_ray, scene, x + aaX*0.5f, y + aaY*0.5f);
					rayTrace(primary_ray, scene, &res);

					pixelColor.x = pixelColor.x + res.color[0];
					pixelColor.y = pixelColor.y + res.color[1];
					pixelColor.z = pixelColor.z + res.color[2];
				}
			}

			pixelColor = pixelColor / 9.0f;
			pixelColor = clamping(pixelColor);

			img.setPixel(x, scene->cam.mHeight-1-y, pixelColor);
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
		if (scene->mObjects[i]->intersect(&ray) > 0)
		{
			if (closest_intersection == true)
			{
				if (scene->mObjects[i]->intersect(&ray) < t)
				{
					t = scene->mObjects[i]->intersect(&ray);
					intersection = scene->mObjects[i];
					continue;
				}
			}
			else
			{
				intersection = scene->mObjects[i];
				break;
			}
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
		float t = intersection->intersect(&ray);
		vec3 i_out, obj_norm;

		i_out = intersection->amb*scene->mLights[0]->amb + intersection->amb*scene->mLights[1]->amb;
		obj_norm = intersection->objectNorm(&ray);

		for (int j = 0; j < scene->mLights.size(); j++)
		{
			CRay shadow_ray;
			shadow_ray.pos = ray.pos + t * ray.dir;
			shadow_ray.dir = normalize(scene->mLights[j]->pos - shadow_ray.pos);
			shadow_ray.pos = shadow_ray.pos + 0.001f * shadow_ray.dir;

			CSceneObject *shadow = findIntersection(shadow_ray, scene, false);
			if (shadow == NULL)
			{
				vec3 i_dif, i_spe, h;

				h = normalize(shadow_ray.dir + normalize(-ray.dir));

				i_dif = intersection->diff * scene->mLights[j]->diff * dot(shadow_ray.dir, obj_norm);
				i_spe = intersection->spec * scene->mLights[j]->spec * pow(dot(obj_norm, h), intersection->shininess);

				i_dif = clamping(i_dif);
				i_spe = clamping(i_spe);

				i_out = i_out + res->energy * i_dif + res->energy * i_spe;
			}
			i_out = clamping(i_out);		
		}

		res->color[0] += i_out.x;
		res->color[1] += i_out.y;
		res->color[2] += i_out.z;
		
		res->energy = res->energy/2.0f;

		if (intersection->absorption > 0.0f)
		{
			CRay refraction_ray;
			refraction_ray.pos = ray.pos + t * ray.dir;
			refraction_ray.dir = normalize((ray.dir - obj_norm * dot(ray.dir, obj_norm)) / intersection->refraction - obj_norm * sqrt(1 - (1 - pow(dot(ray.dir, obj_norm), 2)) / pow(intersection->refraction, 2)));
			refraction_ray.pos = refraction_ray.pos + 0.001f * refraction_ray.dir;

			t = intersection->intersect(&refraction_ray);

			CRay refracted_ray;
			refracted_ray.pos = refraction_ray.pos + t * refraction_ray.dir + 0.001f * ray.dir;
			refracted_ray.dir = ray.dir;

			rayTrace(refracted_ray, scene, res);
		}

		if (res->energy >= 0.1f && intersection->reflect>0.0f)
		{
			CRay secondary_ray;
			secondary_ray.pos = ray.pos + t * ray.dir;
			secondary_ray.dir = reflect(ray.dir, intersection->objectNorm(&ray));
			secondary_ray.pos = secondary_ray.pos + 0.001f * secondary_ray.dir;
			rayTrace(secondary_ray, scene, res);
		}

		return 0;
	}
}