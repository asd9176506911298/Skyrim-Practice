#include <Windows.h>
#include <iostream>
#include "hook.h"
#include "hooks_reclass.h"
#include "draw.h"
#include "hack.h"

#include <d3d9.h>
#include <d3dx9.h>
#include "detours\detours.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "detours.lib")

typedef HRESULT(__stdcall* endScene)(IDirect3DDevice9* o_pDevice);
endScene oEndScene;

extern IDirect3DDevice9* pDevice = nullptr;

Hack* hack;

bool init = false;

HRESULT __stdcall hookEndScene(IDirect3DDevice9* o_pDevice)
{
    //std::cout << "Hooked" << std::endl;
    if (o_pDevice)
        pDevice = o_pDevice;

    if(!init)
    { 
        hack = new Hack();
        hack->Init();
        init = true;
    }


    hack->Update();
    
    for (int i = 0; i < 255; i++)
    {
        if(ents[i] != 0)
        { 
            Vec2 vScreen;
            if (hack->WorldToScreen(ents[i]->coords, vScreen))
            {
                DrawText(ents[i]->nameptr->name, vScreen.x, vScreen.y, D3DCOLOR_ARGB(255, 255, 0, 0));
                /*DrawFillRect(vScreen, 5, 5, D3DCOLOR_ARGB(255, 255, 255, 255));*/
            }
        }
    }

    //DrawFillRect(25, 25, 100, 100, D3DCOLOR_ARGB(255, 255, 255, 255));

    return oEndScene(pDevice);
}

void CreateD3D9Device()
{
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D)
        return;

    D3DPRESENT_PARAMETERS d3dparams{};
    d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dparams.hDeviceWindow = GetForegroundWindow();
    d3dparams.Windowed = true;

    IDirect3DDevice9* pDevice = nullptr;

    HRESULT result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dparams.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dparams, &pDevice);

    if (FAILED(result) || !pDevice)
    {
        pD3D->Release();
        return;
    }

    void** vTable = *reinterpret_cast<void***>(pDevice);
    oEndScene = (endScene)vTable[42];

    pDevice->Release();
    pD3D->Release();
}

DWORD WINAPI HackThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(0);
    uintptr_t hookAddr = moduleBase + 0x2B5CF0;

    entListJmpBack = hookAddr + 0x6;

    Detour32((BYTE*)hookAddr, (BYTE*)entityhook, 6);
    CreateD3D9Device();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)oEndScene, (PVOID)hookEndScene);
    DetourTransactionCommit();

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

