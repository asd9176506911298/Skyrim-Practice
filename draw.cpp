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

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
{
	if (!hack->LineL)
		D3DXCreateLine(pDevice, &hack->LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	hack->LineL->SetWidth(thickness);
	hack->LineL->Draw(Line, 2, color);
}

void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color)
{
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawESPBox2D(Vec2 pos, float size, int thickness, D3DCOLOR color)
{
	Vec2 tl, tr;
	tl.x = pos.x - size;
	tr.x = pos.x + size;
	tl.y = tr.y = pos.y - size;

	Vec2 bl, br;
	bl.x = pos.x - size;
	br.x = pos.x + size;
	bl.y = br.y = pos.y + size;

	DrawLine(tl, tr, thickness, color);
	DrawLine(bl, br, thickness, color);
	DrawLine(tl, bl, thickness, color);
	DrawLine(tr, br, thickness, color);
}