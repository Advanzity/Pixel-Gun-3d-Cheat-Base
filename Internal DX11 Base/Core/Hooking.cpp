#include "../pch.h"
#include "Hooking.h"

namespace DX11Base {
    namespace Core {
        
        // Global instance definition
        std::unique_ptr<Hooking> g_Hooking;
        
        Hooking::Hooking()
        {
            MH_Initialize();
        }

        Hooking::~Hooking()
        {
            DisableAllHooks();
            RemoveAllHooks();
            MH_Uninitialize();
        }

        void Hooking::Initialize()
        {
            EnableAllHooks();
        }

        void Hooking::Shutdown()
        {
            RemoveAllHooks();
        }

        bool Hooking::CreateHook(LPVOID lpTarget, LPVOID pDetour, LPVOID* pOrig)
        {
            if (MH_CreateHook(lpTarget, pDetour, pOrig) != MH_OK || MH_EnableHook(lpTarget) != MH_OK)
                return false;
            return true;
        }

        void Hooking::EnableHook(LPVOID lpTarget) { 
            MH_EnableHook(lpTarget); 
        }

        void Hooking::DisableHook(LPVOID lpTarget) { 
            MH_DisableHook(lpTarget); 
        }

        void Hooking::RemoveHook(LPVOID lpTarget) { 
            MH_RemoveHook(lpTarget); 
        }

        void Hooking::EnableAllHooks() { 
            MH_EnableHook(MH_ALL_HOOKS); 
        }

        void Hooking::DisableAllHooks() { 
            MH_DisableHook(MH_ALL_HOOKS); 
        }

        void Hooking::RemoveAllHooks() { 
            MH_RemoveHook(MH_ALL_HOOKS); 
        }
    }
}
