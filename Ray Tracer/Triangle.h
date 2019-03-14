#pragma once

#include <glm\glm.hpp>
#include "FreeImage.h"

class CTriangle : public CSceneObject
{
public:
	CTriangle(void);
	~CTriangle(void);

	glm::vec3 p0, p1, p2;

	float intersect(CRay* ray);
	glm::vec3 objectNorm(CRay* ray);
};

