#include "pch.h"
#include "WeaponHooks.h"
#include "IL2CPP/IL2CPPHelper.h"
#include "../Features/GameplayFeatures.h"
#include "../Features/RenderingFeatures.h"

namespace DX11Base {
    namespace GameHooks {
        
        void* WeaponHooks::orWeaponSounds = nullptr;
        
        void __fastcall WeaponHooks::WeaponSounds_Hook(IL2CPP::Object* obj)
        {
            if (!obj) return;
            
            // Apply features from our organized feature system
            ApplyGameplayFeatures(obj);
            ApplyRenderingFeatures(obj);
            
            // Call original function
            if (orWeaponSounds)
            {
                ((void(__fastcall*)(IL2CPP::Object*))orWeaponSounds)(obj);
            }
        }
        
        void WeaponHooks::ApplyGameplayFeatures(IL2CPP::Object* obj)
        {
            auto* gameplayFeatures = Features::g_GameplayFeatures.get();
            if (!gameplayFeatures) return;
            
            // Weapon Features
            if (gameplayFeatures->TestBool)
            {
                IL2CPPHelper::SetMemberValue<bool>(obj, "bazooka", true);
            }
            
            if (gameplayFeatures->explosivebullet)
            {
                IL2CPPHelper::SetMemberValue<bool>(obj, "bulletExplode", true);
            }
            
            if (gameplayFeatures->killpointshack)
            {
                IL2CPPHelper::SetMemberValue<bool>(obj, "isBuffPoints", true);
                IL2CPPHelper::SetMemberValue<float>(obj, "buffPointsOther", 1337);
                IL2CPPHelper::SetMemberValue<float>(obj, "buffBonusPointsForKill", 1337);
                IL2CPPHelper::SetMemberValue<float>(obj, "buffPointsAssistDesigner", 1337);
            }
            
            if (gameplayFeatures->shotgunmode)
            {
                IL2CPPHelper::SetMemberValue<bool>(obj, "isShotgun", true);
            }
            
            if (gameplayFeatures->MagicBullet)
            {
                IL2CPPHelper::SetMemberValue<bool>(obj, "isRoundMelee", true);
                IL2CPPHelper::SetMemberValue<float>(obj, "radiusRoundMelee", 99999);
            }
            
            if (gameplayFeatures->doublejump)
            {
                IL2CPPHelper::SetMemberValue<bool>(obj, "isDoubleJump", true);
            }
            
            if (gameplayFeatures->infammo)
            {
                IL2CPPHelper::SetMemberValue<bool>(obj, "isUnlimitedAmmo", true);
            }
        }
        
        void WeaponHooks::ApplyRenderingFeatures(IL2CPP::Object* obj)
        {
            auto* renderingFeatures = Features::g_RenderingFeatures.get();
            if (!renderingFeatures) return;
            
            if (renderingFeatures->bighead)
            {
                IL2CPPHelper::SetMemberValue<bool>(obj, "isHeadMagnifier", true);
                IL2CPPHelper::SetMemberValue<float>(obj, "headMagnifierTime", INFINITY);
            }
        }
        
        void WeaponHooks::InitializeHooks()
        {
            MH_Initialize();
            HookC("WeaponSounds", "Update", &WeaponSounds_Hook, &orWeaponSounds);
        }
        
        void WeaponHooks::ShutdownHooks()
        {
            if (orWeaponSounds)
            {
                MH_DisableHook(orWeaponSounds);
                MH_RemoveHook(orWeaponSounds);
                orWeaponSounds = nullptr;
            }
        }
    }
}
