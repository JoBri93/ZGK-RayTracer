#pragma once
class CScene
{
public:

	CCamera cam;

	std::vector<CSceneObject*> mObjects;

	std::vector<CLight*> mLights;

	CScene(void);
	~CScene(void);

	bool parse(std::string fname);
};

