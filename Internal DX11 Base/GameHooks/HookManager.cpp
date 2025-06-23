#include "../pch.h"
#include "HookManager.h"

namespace DX11Base {
    namespace GameHooks {
        
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
