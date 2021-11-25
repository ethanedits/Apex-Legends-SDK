#pragma once
#include <Windows.h>

namespace Globals {
	HWND hWnd = NULL;
	uintptr_t modBase = NULL;
	HANDLE processID = NULL;
	int killKey = VK_END;
}

namespace MenuSettings {
	bool espEnabled = false;
	bool glowEnabled = false;
	bool noRecoilEnabled = false;
	bool aimbotEnabled = false;
	int aimbotSmoothness = 3;
	bool healthbarEnabled = false;
	bool fovCircle = false;
	int aimbotFOV = 300;
	bool snaplinesEnabled = false;
	bool userScriptsEnabled = false;
}