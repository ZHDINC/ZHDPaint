#include"ColorPalette.h"

ColorPalette::ColorPalette()
{
	// For starters, creating the three primary colors with the intention to use them as brushes.
	AddToColorPalette(RGB(255, 0, 0));
	AddToColorPalette(RGB(0, 255, 0));
	AddToColorPalette(RGB(0, 0, 255));
}

void ColorPalette::DrawColorPalette(HDC hdc)
{
	HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
	int drawiteration = 0;
	for (int row = 0; row < 100; row += 33)
	{
		for (int column = 0; column < 200; column += 25)
		{
			if (drawiteration < colors.size())
			{
				hbrush = CreateSolidBrush(colors[drawiteration].GetPaletteBrush());
				SelectObject(hdc, hbrush);
			}
			
			Rectangle(hdc, column, row, column + 25, row + 33);
			SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
			drawiteration++;
		}
	}
}

void ColorPalette::AddToColorPalette(COLORREF color)
{
	colors.push_back(color);
	if (iteration % 8 == 0 && iteration != 0)
	{
		row += 33;
		column = 0;
	}
	points.push_back(POINT{ column, row });
	column += 25;
	iteration++;
}