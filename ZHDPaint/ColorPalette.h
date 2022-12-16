#pragma once
#include<Windows.h>
#include<vector>
#include"PaletteBrush.h"
#include"Identifiers.h"

class ColorPalette
{
	std::vector<PaletteBrush> colors;
	std::vector<POINT> points;
	int iteration = 0, row = 0, column = 0;
public:
	ColorPalette();
	void AddToColorPalette(COLORREF color);
	void DrawColorPalette(HDC hdc);
	std::vector<PaletteBrush>& GetColors()
	{
		return colors;
	}
	COLORREF GetSpecificIndex(int i)
	{
		return colors[i].GetPaletteBrush();
	}
	PaletteBrush HitCheck(LPARAM lparam)
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		int loopiterator = 0;
		for (POINT p : points)
		{
			if (x > p.x && x < (p.x + 25))
			{
				if (y > p.y && y < (p.y + 33))
				{
					return colors[loopiterator];
				}
			}
			loopiterator++;
		}
	}
};

