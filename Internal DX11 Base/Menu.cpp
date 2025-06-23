#include "pch.h"
#include "Engine.h"
#include "Menu.h"
#include "Features/FeatureManager.h"
ImGuiWindowFlags Flags = 0;

namespace DX11Base {

	void Menu::Render() 
	{
		if (g_Engine->bShowMenu)
		{
			DrawMenu();
		}
	}

	void Menu::DrawMenu()
	{
		if (ImGui::Begin("BNM/DX11 Test ", NULL, Flags))
		{
			if (ImGui::BeginTabBar("tabs"))
			{
				if (ImGui::BeginTabItem("GamePlay"))
				{
					auto* gameplayFeatures = DX11Base::Features::g_GameplayFeatures.get();
					if (gameplayFeatures) {
						ImGui::SmallCheckbox("Bazookaaa", &gameplayFeatures->TestBool);
						ImGui::SmallCheckbox("Explosive Bullet", &gameplayFeatures->explosivebullet);
						ImGui::SmallCheckbox("Points Hack", &gameplayFeatures->killpointshack);
						ImGui::SmallCheckbox("Infinite Ammo", &gameplayFeatures->infammo);
						ImGui::SmallCheckbox("Magic Bullet", &gameplayFeatures->MagicBullet);
						ImGui::SmallCheckbox("doublejump", &gameplayFeatures->doublejump);
						ImGui::SmallCheckbox("ShotGun Mode", &gameplayFeatures->shotgunmode);
						ImGui::SmallCheckbox("Rapid Fire", &gameplayFeatures->RapidFire);
						ImGui::Spacing();
						ImGui::SetNextItemWidth(100);
						ImGui::CustomSliderInt("test", &gameplayFeatures->TestInt, 1, 10, 0, NULL);
					}

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Visual"))
				{
					auto* renderingFeatures = DX11Base::Features::g_RenderingFeatures.get();
					if (renderingFeatures) {
						ImGui::SmallCheckbox("Big Heads", &renderingFeatures->bighead);
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Zombie"))
				{
					auto* zombieFeatures = DX11Base::Features::g_ZombieFeatures.get();
					if (zombieFeatures) {
						ImGui::SmallCheckbox("Anti Zombie Spawn", &zombieFeatures->AntiZombieSpawn);
					}
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			ImGui::End();
		}
	}

	void Menu::Loops()
	{

	}
}