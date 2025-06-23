#include "pch.h"
#include "HookManager.h"
#include "WeaponHooks.h"

namespace DX11Base {
    namespace GameHooks {
        
        // Global hook manager instance definition
        std::unique_ptr<HookManager> g_HookManager;
        
        bool HookManager::hooksInitialized = false;
        
        void HookManager::InitializeAllHooks()
        {
            if (hooksInitialized) return;
            
            WeaponHooks::InitializeHooks();
            // Add other hook categories here as they're created
            
            hooksInitialized = true;
        }
        
        void HookManager::ShutdownAllHooks()
        {
            if (!hooksInitialized) return;
            
            WeaponHooks::ShutdownHooks();
            // Add other hook categories here as they're created
            
            hooksInitialized = false;
        }
    }
}
