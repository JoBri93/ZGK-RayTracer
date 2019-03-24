#include "StdAfx.h"
#include "Sphere.h"


CSphere::CSphere(void)
{
}


CSphere::~CSphere(void)
{
}

float CSphere::intersect(CRay* ray)
{
	float t = -1;
	glm::vec3 v=ray->pos-this->center;
	float A=(ray->dir.x)*(ray->dir.x)+(ray->dir.y)*(ray->dir.y)+ (ray->dir.z)*(ray->dir.z);
	float B=2*(v.x*ray->dir.x+ v.y*ray->dir.y+ v.z*ray->dir.z);
	float C=v.x*v.x+v.y*v.y+v.z*v.z-this->radius*this->radius;
	float delta = B * B - 4 * A*C;
	if (delta > 0)
	{
		float t1, t2;
		t1 = (-B - sqrtf(delta)) / (2 * A);
		t2 = (-B + sqrtf(delta)) / (2 * A);

		if (t1 > 0.0f && t2 > 0.0f)
		{
			if (t1 < t2) t = t1;
			else t = t1;
		}
		else if (t1 < 0.0f && t2 > 0.0f)
		{
			t = t2;
		}
		else if (t1 > 0.0f && t2 < 0.0f)
		{
			t = t1;
		}
	}
	if (t == 0)
		return 0.001;
	else
		return t;
}

glm::vec3 CSphere::objectNorm(CRay* ray)
{
	glm::vec3 norm = normalize(ray->pos+intersect(ray)*ray->dir - this->center);
	return norm;
}