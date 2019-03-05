#pragma once
class CTriangle : public CSceneObject
{
public:
	CTriangle(void);
	~CTriangle(void);

	//Wierzcholek 1, 2, 3

	float intersect(CRay* ray);
};

