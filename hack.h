#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

struct Vec2
{
	float x, y;
};

struct Vec3
{
	float x, y, z;
};

struct Vec4
{
	float x, y, z, w;
};

class Hack
{
public:
	uintptr_t moduleBase = 0x0;
	uintptr_t dwViewMatrix = 0x0;

	ID3DXFont* FontF;

	int windowWidth = 1280;
	int windowHeight = 720;
	float viewMatrix[16];

	void Init();
	void Update();
	bool WorldToScreen(Vec3 pos, Vec2& screen);
};