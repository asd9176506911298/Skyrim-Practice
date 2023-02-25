#include <Windows.h>
#include <iostream>
#include "hook.h"
#include "hooks_reclass.h"
#include "draw.h"
#include "hack.h"
#include <string>

#include <d3d9.h>
#include <d3dx9.h>
#include "detours\detours.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "detours.lib")

#define abs(x, y)

typedef HRESULT(__stdcall* endScene)(IDirect3DDevice9* o_pDevice);
endScene oEndScene;
BYTE patchByte[6]{};
BYTE NoClipByte[6]{};

extern IDirect3DDevice9* pDevice = nullptr;

Hack* hack;

bool init = false;
bool noclip = false;
float relx, rely, relz, newx, newy, newz;

float calculateDistance(Vec3 pos)
{
    return sqrt(pow(ents[0]->coords.x - pos.x, 2) + pow(ents[0]->coords.y - pos.y, 2) + pow(ents[0]->coords.z - pos.z, 2));
}

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
        std::cout << ents[0] << std::endl;
    }


    hack->Update();
    
    for (int i = 0; i < 255; i++)
    {
        if(ents[i] != 0)
        { 
            Vec2 vScreen;
            if (calculateDistance(ents[i]->coords) > 4000 || i == 0)
                continue;

            if (hack->WorldToScreen(ents[i]->coords, vScreen))
            {
                std::string s = std::to_string(i);
                char const* pchar = s.c_str();
                //DrawText(ents[i]->nameptr->name, vScreen.x, vScreen.y, D3DCOLOR_ARGB(255, 255, 0, 0));
                DrawText(pchar, vScreen.x, vScreen.y, D3DCOLOR_ARGB(255, 255, 0, 0));
                //DrawFillRect(vScreen, 20, 20, D3DCOLOR_ARGB(255, 255, 0, 0));
                DrawESPBox2D(vScreen, 20, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
                /*DrawFillRect(vScreen, 5, 5, D3DCOLOR_ARGB(255, 255, 255, 255));*/
            }
        }
    }
    float noclipSpeed = 0.02f;
    if (GetAsyncKeyState(VK_F1) & 1)
        noclip = !noclip;

    

    if(noclip)
    {
        relx = 0.0f;
        rely = 0.0f;
        relz = 0.0f;

        Axis = zAxisPtr + 0xE8;
        if (GetAsyncKeyState(VK_SPACE))
        {
            *(float*)Axis += 0.02;
        }

        if (GetAsyncKeyState(VK_NUMPAD6))
        {
            relx += noclipSpeed;
        }
        if (GetAsyncKeyState(VK_NUMPAD4))
        {
            relx -= noclipSpeed;
        }

        if (GetAsyncKeyState(VK_NUMPAD8))
        {
            relz += noclipSpeed;
        }
        if (GetAsyncKeyState(VK_NUMPAD2))
        {
            relz -= noclipSpeed;
        }
    

        newx = hack->viewMatrix[0]*relx + hack->viewMatrix[4] * rely + hack->viewMatrix[8] * relz;
        newy = hack->viewMatrix[1]*relx + hack->viewMatrix[5] * rely + hack->viewMatrix[9] * relz;
        newz = hack->viewMatrix[2]*relx + hack->viewMatrix[6] * rely + hack->viewMatrix[10] * relz;

        Axis = zAxisPtr + 0xE0;
        *(float*)Axis += newx;
        Axis += 0x4;
        *(float*)Axis += newy;
        //Axis += 0x4;
        //*(float*)Axis += newz;
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
    uintptr_t NoclipAddr = moduleBase + 0x98CAAC;

    memcpy(patchByte, (void*)hookAddr, 6);
    memcpy(NoClipByte, (void*)NoclipAddr, 6);

    entListJmpBack = hookAddr + 0x6;
    noClipJmpBack = NoclipAddr + 0x6;

    //hook
    Detour32((BYTE*)hookAddr, (BYTE*)entityhook, 6);
    Detour32((BYTE*)NoclipAddr, (BYTE*)Nocliphook, 6);
    CreateD3D9Device();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)oEndScene, (PVOID)hookEndScene);
    DetourTransactionCommit();

    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(1);
    }

    //unhook
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)oEndScene, hookEndScene);
    DetourTransactionCommit();
    
    Sleep(200);

    DWORD oldProctect;
    VirtualProtect((LPVOID)hookAddr, 6, PAGE_EXECUTE_READWRITE, &oldProctect);
    memcpy((void*)hookAddr, (char*)patchByte, 6);
    VirtualProtect((LPVOID)hookAddr, 6, oldProctect, &oldProctect);

    VirtualProtect((LPVOID)NoclipAddr, 6, PAGE_EXECUTE_READWRITE, &oldProctect);
    memcpy((void*)NoclipAddr, (char*)NoClipByte, 6);
    VirtualProtect((LPVOID)NoclipAddr, 6, oldProctect, &oldProctect);



    FreeConsole();
    fclose(f);
    FreeLibraryAndExitThread(hModule, 0);

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

