#pragma once

#include <fstream>
#include <sstream>

using namespace std;

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

