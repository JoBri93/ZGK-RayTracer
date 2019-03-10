#pragma once
class CSceneObject
{
public:
	CSceneObject(void);
	~CSceneObject(void);

	int reflect;
	int shininess;
	glm::vec3 amb;
	glm::vec3 diff;
	glm::vec3 spec;


	virtual float intersect( CRay* ray ) = 0;
};

