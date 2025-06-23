#pragma once
#include "WeaponHooks.h"

namespace DX11Base {
    namespace GameHooks {
        
        class HookManager {
        public:
            static void InitializeAllHooks();
            static void ShutdownAllHooks();
            
        private:
            static bool hooksInitialized;
        };
        
        // Global hook manager instance
        inline std::unique_ptr<HookManager> g_HookManager;
    }
}
