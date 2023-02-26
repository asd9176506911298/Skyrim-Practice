#pragma once
#include <Windows.h>
#include "hack.h"

extern DWORD entListJmpBack = 0x0;
extern DWORD noClipJmpBack = 0x0;
DWORD Axis = 0x0;
DWORD zAxisPtr = 0x0;
class playerent
{
public:
	char pad_0000[36]; //0x0000
	class nameptrClass* nameptr; //0x0024
	char pad_0028[12]; //0x0028
	Vec3 coords; //0x0034
	char pad_0040[280]; //0x0040
	float health; //0x0158
	char pad_015C[20]; //0x015C
	float stamina; //0x0170
	char pad_0174[20]; //0x0174
	class boneptr1class* boneptr1; //0x0188
}; //Size: 0x018C

class nameptrClass
{
public:
	char pad_0000[136]; //0x0000
	char* name; //0x0088
}; //Size: 0x008C

class boneptr1class
{
public:
	char pad_0000[4]; //0x0000
	class boneptr2class* boneptr2; //0x0004
}; //Size: 0x0008

class boneptr2class
{
public:
	char pad_0000[192]; //0x0000
	class boneptr3class* boneptr3; //0x00C0
}; //Size: 0x00C4

class boneClass
{
public:
	char pad_0000[88]; //0x0000
	Vec3 coords; //0x0058
	char pad_0064[16]; //0x0064
	char* nameptr; //0x0074
}; //Size: 0x00780x78);

class boneptr3class
{
public:
	class boneClass boneArray[80]; //0x0000
}; //Size: 0x04B0

__declspec(naked) void Nocliphook()
{
	__asm {
		mov zAxisPtr, esi
		mov edx, [edx + 0x78]
		fld dword ptr[edx + 0xc]
		pushad
	}

	__asm {
		popad
		jmp [noClipJmpBack]
	}
}
playerent* ents[255];
playerent* entptr;
int index;

bool alreadyThere = false;

__declspec(naked) void entityhook()
{
	__asm {
		mov eax,[esi+0x34]
		mov ecx,[esi + 0x38]
		mov entptr, esi
		pushad
	}

	if (entptr == nullptr)
	{
		goto GIVE_UP;
	}

	alreadyThere = false;

	for (index = 0; index < 100; index++)
	{
		if (ents[index] == entptr)
		{
			alreadyThere = true;
			break;
		}
	}

	if (alreadyThere)
	{
		goto GIVE_UP;
	}
	else
	{
		for (index = 0; index < 100; index++)
		{
			if (ents[index] == 0)
			{
				ents[index] = entptr;
				break;
			}
		}
	}

GIVE_UP:
	__asm{
		popad
		jmp [entListJmpBack]
	}
}