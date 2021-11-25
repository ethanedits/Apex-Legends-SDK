#pragma once
#include "Drawings.h"
#include "../../../imgui/imgui.h"
#include "../../../imgui/imgui_impl_dx9.h"
#include "../../../imgui/imgui_impl_win32.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

extern HWND overlayWindow;
extern IDirect3D9Ex* p_Object;
extern IDirect3DDevice9Ex* p_Device;
extern D3DPRESENT_PARAMETERS p_Params;
extern ID3DXLine* p_Line;
extern POINT windowWH;

namespace overlay
{
	bool InitWindow();
	bool DirectXInit();
	void Render();
	void DirectXShutdown();
	//void UpdateWindowPos();
}