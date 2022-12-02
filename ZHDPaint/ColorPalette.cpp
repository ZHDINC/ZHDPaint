#include"ColorPalette.h"

ColorPalette::ColorPalette()
{
	// For starters, creating the three primary colors with the intention to use them as brushes.
	colors.push_back(PaletteBrush(RGB(255,0,0)));
	colors.push_back(PaletteBrush(RGB(0, 255, 0)));
	colors.push_back(PaletteBrush(RGB(0, 0, 255)));
}

void ColorPalette::DrawColorPalette(HDC hdc, int left, int top, int right, int bottom)
{
	/*Rectangle(hdc, left, top, right, bottom);
	int size = (right - left) / colors.size();
	int iteration = 0;
	PaletteBrush currentColor{ RGB(0,0,0), false };
	for (PaletteBrush color : colors)
	{
		HBRUSH hbrush = CreateSolidBrush(color.GetPaletteBrush());
		SelectObject(hdc, hbrush);
		if (color.GetState())
		{
			currentColor = color;
			HPEN hpen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
			SelectObject(hdc, hpen);
		}
		Rectangle(hdc, left + size * iteration, top, right, bottom);
		iteration++;
		SelectObject(hdc, (HPEN)GetStockObject(BLACK_PEN));
	}*/
}