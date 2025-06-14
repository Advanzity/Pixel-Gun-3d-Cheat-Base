#include "pch.h"
#include "Engine.h"
#include "Menu.h"
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
					ImGui::SmallCheckbox("Bazookaaa", &g_Engine->TestBool);
					ImGui::SmallCheckbox("Explosive Bullet", &g_Engine->explosivebullet);
					ImGui::SmallCheckbox("Points Hack", &g_Engine->killpointshack);
					ImGui::SmallCheckbox("Infinite Ammo", &g_Engine->infammo);
					ImGui::SmallCheckbox("Magic Bullet", &g_Engine->MagicBullet);
					ImGui::SmallCheckbox("doublejump", &g_Engine->doublejump);
					ImGui::SmallCheckbox("ShotGun Mode", &g_Engine->shotgunmode);
					ImGui::SmallCheckbox("Big Heads", &g_Engine->bighead);
					ImGui::Spacing();
					ImGui::SetNextItemWidth(100);
					ImGui::CustomSliderInt("test", &g_Engine->TestInt, 1, 10, 0, NULL);

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Zombie"))
				{
					ImGui::SmallCheckbox("Anti Zombie Spawn", &g_Engine->AntiZombieSpawn);
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