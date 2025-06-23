#pragma once
#include "pch.h"
#include "Engine.h"
#include "Menu.h"
#include "GameHooks/IL2CPP/JQ-BNM-main/BNM-IL2CPP/BNM.hpp"

using namespace DX11Base;

void ClientBGThread()
{
    while (g_Running)
    {
        Menu::Loops();

        if (g_KillSwitch)
        {
            Core::g_D3D11Window->UnhookD3D();
            Core::g_Hooking->Shutdown();
            g_Engine.release();     //  releases all created class instances
            g_Running = false;

        }

        std::this_thread::sleep_for(1ms);
        std::this_thread::yield();
    }
}

DWORD WINAPI MainThread_Initialize(LPVOID dwModule) {

    UNREFERENCED_PARAMETER(dwModule);
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);;
    SetConsoleTitleA("Made By 0SD47");
    const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    g_Engine = std::make_unique<Engine>();
    Core::g_D3D11Window->HookD3D();
    Core::g_Hooking->Initialize();
    IL2CPP::Initialize();
    // Game hooks are now automatically initialized by the Engine constructor

    std::thread WCMUpdate(ClientBGThread);

    //  RENDER LOOP
    g_Running = true;
    static int LastTick = 0;
    while (g_Running)
    {
        if ((GetAsyncKeyState(VK_INSERT)) && ((GetTickCount64() - LastTick) > 500))
        {
            g_Engine->bShowMenu = !g_Engine->bShowMenu;
            LastTick = GetTickCount64();
        }
        if ((GetAsyncKeyState(VK_END))) 
        {
            g_KillSwitch = true;
        }
        std::this_thread::sleep_for(1ms);
        std::this_thread::yield();
    }


    //  EXIT
    WCMUpdate.join();
    FreeLibraryAndExitThread(g_hModule, EXIT_SUCCESS);
    if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK) {
        return 1;
    }
    if (MH_Uninitialize() != MH_OK) {
        return 1;
    }
    return EXIT_SUCCESS;
}