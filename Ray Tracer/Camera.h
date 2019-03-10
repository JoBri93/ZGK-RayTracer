#pragma once

#include <glm\glm.hpp>
#include "FreeImage.h"

class CCamera
{
public:
	int mWidth;
	int mHeight;
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	float FOV;

	CCamera(void);
	~CCamera(void);
};

