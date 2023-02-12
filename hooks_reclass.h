#pragma once
#include <Windows.h>
#include "vec3.h"

extern DWORD entListJmpBack = 0x0;

DWORD EntityObjStart = 0x0;

class playerent
{
public:
	char pad_0000[36]; //0x0000
	class nameptrClass* nameptr; //0x0024
	char pad_0028[12]; //0x0028
	vec3 coords; //0x0034
}; //Size: 0x0040

class nameptrClass
{
public:
	char pad_0000[136]; //0x0000
	char* name; //0x0088
}; //Size: 0x008C

playerent* ents[100];
playerent* entptr;
int index;

bool alreadyThere = false;

__declspec(naked) void entityhook()
{
	__asm {
		mov eax,[esi+0x34]
		mov ecx,[esi + 0x38]
		mov EntityObjStart, esi
		pushad
	}

	__asm {
		mov eax,EntityObjStart
		mov entptr, eax
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