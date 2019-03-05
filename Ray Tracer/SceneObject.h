#pragma once
class CSceneObject
{
public:
	CSceneObject(void);
	~CSceneObject(void);

	virtual float intersect( CRay* ray ) = 0;
};

