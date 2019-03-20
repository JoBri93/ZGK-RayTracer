#include "StdAfx.h"
#include "Triangle.h"


CTriangle::CTriangle(void)
{
}


CTriangle::~CTriangle(void)
{
}

float CTriangle::intersect(CRay* ray)
{
	float t;

	glm::vec3 norm = normalize(cross(p0 - p1, p2 - p1));
	float A = norm.x;
	float B = norm.y;
	float C = norm.z;
	float D = glm::dot(norm, p1);
	t = -(A*ray->pos.x + B * ray->pos.y + C * ray->pos.z - D) / (A*ray->dir.x + B * ray->dir.y + C * ray->dir.z);

	glm::vec3 v0, v1, v2;
	v0 = p0 - p1;
	v1 = p2 - p1;
	v2 = ray->pos + t * ray->dir - p1;

	float dot00, dot01, dot02, dot11, dot12;
	dot00 = glm::dot(v0, v0);
	dot01 = glm::dot(v0, v1);
	dot02 = glm::dot(v0, v2);
	dot11 = glm::dot(v1, v1);
	dot12 = glm::dot(v1, v2);

	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u, v;
	u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	if (u >= 0 && v >= 0 && u + v < 1.0f)
		return t;
	else
		return -1;
}

glm::vec3 CTriangle::objectNorm(CRay* ray)
{
	glm::vec3 norm = normalize(cross(p1 - p0, p2 - p1));
	return norm;
}