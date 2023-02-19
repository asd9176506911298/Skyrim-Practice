#pragma once
#include <d3d9.h>
#include <d3dx9.h>

extern IDirect3DDevice9* pDevice;

void DrawFillRect(int x, int y, int w, int h, D3DCOLOR color);