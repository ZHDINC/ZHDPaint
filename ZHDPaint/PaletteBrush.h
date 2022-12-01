#pragma once
#include<Windows.h>

class PaletteBrush
{
	COLORREF color;
	bool isActive;
public:
	PaletteBrush(COLORREF color, bool isActive)
		: color{ color }, isActive{ isActive }
	{

	}
	void SetActive() { isActive = true; }
	void SetInactive() { isActive = false; }
	COLORREF GetPaletteBrush() { return color; }
	bool GetState() { return isActive; }
};