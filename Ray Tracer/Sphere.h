#pragma once

#include <glm\glm.hpp>
#include "FreeImage.h"

class CSphere : public CSceneObject
{
public:
	CSphere(void);
	~CSphere(void);

	float radius;
	glm::vec3 center;

	float intersect(CRay* ray);
	glm::vec3 objectNorm(CRay* ray);
};

