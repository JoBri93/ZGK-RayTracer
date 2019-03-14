#include "StdAfx.h"
#include "Bitmap.h"

CBitmap::CBitmap(void)
{
}

CBitmap::~CBitmap(void)
{
}

bool CBitmap::init(int width, int height)
{
	mBitmap = FreeImage_AllocateT(FIT_BITMAP, width, height, 32);
	return true;
}

bool CBitmap::save(std::string fname)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fname.c_str());
	

	FreeImage_Save(FIF_BMP, mBitmap, fname.c_str());
	return true;
}

bool CBitmap::setPixel(int x, int y, glm::vec3 color)
{
	RGBQUAD quad;

	if (color.x < 0.00304) quad.rgbRed = color.x*12.92*255;
	else quad.rgbRed = (1.055*pow(color.x,1/2.4)-0.055)*255;

	if (color.y < 0.00304) quad.rgbGreen = color.y*12.92*255;
	else quad.rgbGreen = (1.055*pow(color.y, 1 / 2.4) - 0.055)*255;

	if (color.z < 0.00304) quad.rgbBlue = color.z*12.92*255;
	else quad.rgbBlue = (1.055*pow(color.z, 1 / 2.4) - 0.055)*255;

	FreeImage_SetPixelColor(mBitmap, x, y, &quad);
	return false;
}
