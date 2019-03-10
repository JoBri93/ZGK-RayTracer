#pragma once

#include <glm\glm.hpp>
#include "FreeImage.h"

class CLight
{
public:
	glm::vec3 pos;
	glm::vec3 amb;
	glm::vec3 diff;
	glm::vec3 spec;

	CLight(void);
	~CLight(void);
};

