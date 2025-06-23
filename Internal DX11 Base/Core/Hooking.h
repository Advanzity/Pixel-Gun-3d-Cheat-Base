#pragma once
#include "../helper.h"

namespace DX11Base {
    namespace Core {
        
        class Hooking
        {
        public:
            void								Initialize();
            void								Shutdown();
            static bool							CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID* pOrig);
            static void							EnableHook(LPVOID pTarget);
            static void							EnableAllHooks();
            static void							DisableHook(LPVOID pTarget);
            static void							RemoveHook(LPVOID pTarget);
            static void							DisableAllHooks();
            static void							RemoveAllHooks();

        public:
            explicit Hooking();
            ~Hooking() noexcept;
        };
        
        // Global instance
        inline std::unique_ptr<Hooking> g_Hooking;
    }
}
