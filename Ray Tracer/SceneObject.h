#pragma once

#include <glm\glm.hpp>
#include "FreeImage.h"

class CSceneObject
{
public:
	CSceneObject(void);
	~CSceneObject(void);

	float reflect;
	float shininess;
	glm::vec3 amb;
	glm::vec3 diff;
	glm::vec3 spec;

	virtual float intersect( CRay* ray ) = 0;
	virtual glm::vec3 objectNorm(CRay* ray) = 0;
};

