#pragma once
#include<Windows.h>
#include<vector>
#include"PaletteBrush.h"

class ColorPalette
{
	std::vector<PaletteBrush> colors;
public:
	ColorPalette();
	void DrawColorPalette(HDC hdc, int left, int top, int right, int bottom);
	std::vector<PaletteBrush>& GetColors()
	{
		return colors;
	}
	COLORREF GetSpecificIndex(int i)
	{
		return colors[i].GetPaletteBrush();
	}
};

