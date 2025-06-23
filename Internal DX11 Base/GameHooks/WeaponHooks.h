#pragma once
#include "IL2CPP/IL2CPPHelper.h"
#include "../Engine.h"
#include "../Features/FeatureManager.h"

namespace DX11Base {
    namespace GameHooks {
        
        class WeaponHooks {
        public:
            static void* orWeaponSounds;
            
            static void __fastcall WeaponSounds_Hook(IL2CPP::Object* obj);
            static void InitializeHooks();
            static void ShutdownHooks();
            
        private:
            static void ApplyGameplayFeatures(IL2CPP::Object* obj);
            static void ApplyRenderingFeatures(IL2CPP::Object* obj);
        };
        
        // Global hook manager
        class HookManager {
        public:
            static void InitializeAllHooks();
            static void ShutdownAllHooks();
        };
    }
}
