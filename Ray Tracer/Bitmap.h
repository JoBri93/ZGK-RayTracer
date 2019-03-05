#pragma once

#include <glm\glm.hpp>
#include "FreeImage.h"

class CBitmap
{
public:
	int mWidth;
	int mHeight;
	FIBITMAP *mBitmap;

	//data
	CBitmap(void);
	~CBitmap(void);

	bool init(int width, int height);
	bool save(std::string fname);

	bool setPixel(int x, int y, glm::vec3 color);
};

