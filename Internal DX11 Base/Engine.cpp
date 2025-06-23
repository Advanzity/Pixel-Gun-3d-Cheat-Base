#include "pch.h"
#include "Engine.h"
#include "Menu.h"
#include "Fonts.h"

namespace DX11Base 
{

	Engine::Engine()
	{
		Core::g_D3D11Window = std::make_unique<Core::D3D11Window>();
		Core::g_Hooking = std::make_unique<Core::Hooking>();
		Features::g_FeatureManager = std::make_unique<Features::FeatureManager>();
		GameHooks::g_HookManager = std::make_unique<GameHooks::HookManager>();
		
		// Initialize all systems
		Features::g_FeatureManager->InitializeAllFeatures();
		GameHooks::HookManager::InitializeAllHooks();
	}

	Engine::~Engine()
	{
		// Shutdown in reverse order
		if (GameHooks::g_HookManager) {
			GameHooks::HookManager::ShutdownAllHooks();
		}
		if (Features::g_FeatureManager) {
			Features::g_FeatureManager->ShutdownAllFeatures();
		}
		Core::g_Hooking.release();
		Core::g_D3D11Window.release();
	}
}