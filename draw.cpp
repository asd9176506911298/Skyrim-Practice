#include "draw.h"

extern Hack* hack;

void DrawFillRect(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT rect = { x,y, x + w, y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawFillRect(Vec2 vScreen, int w, int h, D3DCOLOR color)
{
	DrawFillRect(vScreen.x, vScreen.y, w, h, color);
}

void DrawText(const char* text, float x, float y, D3DCOLOR color)
{
	RECT rect;

	if (!hack->FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &hack->FontF);

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}