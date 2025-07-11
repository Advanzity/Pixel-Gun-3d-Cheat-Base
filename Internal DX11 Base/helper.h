#pragma once
#define DEBUG 1

#include "pch.h"

extern DWORD WINAPI MainThread_Initialize(LPVOID dwModule);

namespace DX11Base
{
	using namespace std::chrono_literals;
	extern HMODULE g_hModule;
	extern std::atomic_bool g_Running;
	extern std::atomic_bool g_KillSwitch;

	struct Vector2
	{
		float x, y;
	};

	struct Vector3
	{
		float x, y, z;
	};

	struct Vector4
	{
		float x, y, z, w;
	};

	struct DVector2
	{
		double x, y;
	};

	struct DVector3
	{
		double x, y, z;
	};

	struct DVector4
	{
		double x, y, z, w;
	};
}