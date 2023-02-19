#pragma once
#include "hack.h"
#include <d3d9.h>
#include <d3dx9.h>

extern IDirect3DDevice9* pDevice;

void DrawFillRect(int x, int y, int w, int h, D3DCOLOR color);
void DrawFillRect(Vec2 vScreen, int w, int h, D3DCOLOR color);
void DrawText(const char* text, float x, float y, D3DCOLOR color);
void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color);
void DrawESPBox2D(Vec2 pos, float size, int thickness, D3DCOLOR color);