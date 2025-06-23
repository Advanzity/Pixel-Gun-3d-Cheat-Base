#pragma once
#include "../Engine.h"
#include "../Features/FeatureManager.h"

// Forward declarations
namespace IL2CPP {
    struct Object;
}

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
    }
}
