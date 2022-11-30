#pragma once
#include<Windows.h>
#include<vector>

class ColorPalette
{
	std::vector<COLORREF> colors;
public:
	ColorPalette();
	void DrawColorPalette(HDC hdc, int left, int top, int right, int bottom);
};