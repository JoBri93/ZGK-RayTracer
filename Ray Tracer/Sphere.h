#pragma once
class CSphere : public CSceneObject
{
public:
	CSphere(void);
	~CSphere(void);

	//Promien
	//Srodek

	float intersect(CRay* ray);
};

