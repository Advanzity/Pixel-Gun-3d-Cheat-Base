#pragma once
#include "helper.h"
#include "Features/FeatureManager.h"
#include "Core/D3D11Window.h"
#include "Core/Hooking.h"
#include "GameHooks/HookManager.h"

namespace DX11Base 
{
	class Engine {
	public:

		HWND	pGameWindow{ 0 };
		bool bShowMenu{ false };
		Engine();
		~Engine();
		
		// Feature access methods
		Features::FeatureManager* GetFeatureManager() { return Features::g_FeatureManager.get(); }
		
		// Core system access methods  
		Core::D3D11Window* GetD3D11Window() { return Core::g_D3D11Window.get(); }
		Core::Hooking* GetHooking() { return Core::g_Hooking.get(); }
		
		// Game hooks access
		GameHooks::HookManager* GetHookManager() { return GameHooks::g_HookManager.get(); }
		
		struct Il2CppClass;
		struct Il2CppObject;
		struct Il2CppString;
	};
	inline std::unique_ptr<Engine> g_Engine;
}