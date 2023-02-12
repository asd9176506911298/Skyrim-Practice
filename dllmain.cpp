#include <Windows.h>
#include "hook.h"
#include "hooks_reclass.h"


DWORD WINAPI HackThread(HMODULE hModule)
{
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(0);
    uintptr_t hookAddr = moduleBase + 0x2B5CF0;

    entListJmpBack = hookAddr + 0x6;

    Detour32((BYTE*)hookAddr, (BYTE*)entityhook, 6);

    return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    DisableThreadLibraryCalls(hModule);
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
    }

    return TRUE;
}

