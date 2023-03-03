#pragma once
#include<Windows.h>
#include"ColorPalette.h"
#include"Identifiers.h"

LRESULT CALLBACK ColorPaletteWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static ColorPalette colorpalette;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	switch (message)
	{
	case WM_PAINT:
	{
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
		int numberOfColors = colorpalette.GetColors().size();
		colorpalette.DrawColorPalette(hdc);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_LBUTTONDOWN:
		SendMessage(GetParent(hwnd), WM_SENDCOLORBRUSH, 0, colorpalette.HitCheck(lparam).GetPaletteBrush());
		return 0;
	case WM_GETCOLORBRUSH:
		SendMessage(GetParent(hwnd), WM_SENDCOLORBRUSH, 0, colorpalette.GetLastColor());
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}