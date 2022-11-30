#include"ColorPalette.h"

ColorPalette::ColorPalette()
{
	// For staters, creating the three primary colors with the intention to use them as brushes.
	colors.push_back(RGB(255, 0, 0));
	colors.push_back(RGB(0, 255, 0));
	colors.push_back(RGB(0, 0, 255));
}

void ColorPalette::DrawColorPalette(HDC hdc, int left, int top, int right, int bottom)
{
	Rectangle(hdc, left, top, right, bottom);
	int size = (right - left) / colors.size();
	int iteration = 0;
	for (COLORREF color : colors)
	{
		HBRUSH hbrush = CreateSolidBrush(color);
		SelectObject(hdc, hbrush);
		Rectangle(hdc, left + size * iteration, top, right, bottom);
		iteration++;
	}
}