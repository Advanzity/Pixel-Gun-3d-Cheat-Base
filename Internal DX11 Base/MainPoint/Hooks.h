#pragma once
#include "IL2CPPHelper.h"
#include "../Engine.h"

void* orWeaponSounds;
void __fastcall WeaponSounds(IL2CPP::Object* obj)
{

    if (DX11Base::g_Engine->TestBool)
    {
        IL2CPPHelper::SetMemberValue<bool>(obj, "bazooka", true);
    }

    if (DX11Base::g_Engine->explosivebullet)
    {
        IL2CPPHelper::SetMemberValue<bool>(obj, "bulletExplode", true);
    }

    if (DX11Base::g_Engine->killpointshack)
    {
        IL2CPPHelper::SetMemberValue<bool>(obj, "isBuffPoints", true);
        IL2CPPHelper::SetMemberValue<float>(obj, "buffPointsOther", 1337);
        IL2CPPHelper::SetMemberValue<float>(obj, "buffBonusPointsForKill", 1337);
        IL2CPPHelper::SetMemberValue<float>(obj, "buffPointsAssistDesigner", 1337);
        
    }
    if (DX11Base::g_Engine->shotgunmode)
    {
        IL2CPPHelper::SetMemberValue<bool>(obj, "isShotgun", true);
    }
    if (DX11Base::g_Engine->MagicBullet)
    {
        IL2CPPHelper::SetMemberValue<bool>(obj, "isRoundMelee", true);
        IL2CPPHelper::SetMemberValue<float>(obj, "radiusRoundMelee", 99999);
    }
    if (DX11Base::g_Engine->bighead)
    {
        IL2CPPHelper::SetMemberValue<bool>(obj, "isHeadMagnifier", true);
        IL2CPPHelper::SetMemberValue<float>(obj, "headMagnifierTime", INFINITY);
    }
    if (DX11Base::g_Engine->doublejump)
    {
        IL2CPPHelper::SetMemberValue<bool>(obj, "isDoubleJump", true);
    }

    if (DX11Base::g_Engine->infammo)
    {
        IL2CPPHelper::SetMemberValue<bool>(obj, "isUnlimitedAmmo", true);
    }


    if (orWeaponSounds)
    {
        ((void(__fastcall*)(IL2CPP::Object*))orWeaponSounds)(obj);
    }
}

void mainhook()
{
    MH_Initialize();

    HookC("WeaponSounds", "Update", &WeaponSounds, &orWeaponSounds);
}
