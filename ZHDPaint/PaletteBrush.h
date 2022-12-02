#pragma once
#include<Windows.h>

class PaletteBrush
{
	COLORREF color;
public:
	PaletteBrush(COLORREF color)
		: color{ color }
	{

	}
	PaletteBrush() { };
	COLORREF GetPaletteBrush() { return color; }
	void SetPaletteBrush(COLORREF sentColor) { color = sentColor; }
};