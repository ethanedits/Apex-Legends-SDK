#include "overlay.h"
#include <string>
#include <filesystem>
#include <thread>
#include "../../../imgui/ksk.h"
#include "../../../imgui/unknownController.h"
#include "../../../imgui/FontAwesome.h"
#include "../../../imgui/IconsFontAwesome.h"
#include "../../../imgui/Comfortaa-Regular.h"
#include "../../../imgui/Comfortaa-Bold.h"
#include "../../../Utilities/globals.h"
#include "../../../Utilities/utils.h"

#include "../../../Utilities/includes.h"
#include "../../../SDK/Entity/BaseEntity.h"
#include "../../../SDK/Entity/LocalEntity.h"
#include "../../../Main/Modules/Modules.h"
#include "../Menu/Menu.h"
#include "../Scene/Scene.h"

// set up these variables here in case we need them later
HWND overlayWindow;
IDirect3D9Ex* p_Object;
IDirect3DDevice9Ex* p_Device;
D3DPRESENT_PARAMETERS p_Params;
ID3DXLine* p_Line;
ID3DXFont* pFont;
ID3DXFont* pModuleFont;
ID3DXFont* pESPFont;
RECT rc;
MARGINS margin = { -1 };
ImFont* titleFont;
ImFont* bigFont;
MSG message;
IDirect3DTexture9* ksk;
IDirect3DTexture9* unknownCtrl;
float oldDelay = 0;

POINT windowWH = { 0, 0 };

#define ARGB(a, r, g, b) D3DCOLOR_ARGB(a, r, g, b)

bool menuActive = true;

// winprochandler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_SIZE:
		if (p_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Params.BackBufferWidth = LOWORD(lParam);
			p_Params.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = p_Device->Reset(&p_Params);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;

	case WM_DESTROY:
		overlay::DirectXShutdown();
		PostQuitMessage(0);
		return 0;
		break;

	default:
		return DefWindowProcW(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void HandleMessages(std::wstring lClassName, std::wstring lWindowName)
{
	// create the window and set window attributes
	overlayWindow = CreateWindowExW(/*WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED*/NULL, lClassName.c_str(), lWindowName.c_str(), WS_POPUP | WS_VISIBLE, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, 0);
	SetWindowLongA(overlayWindow, GWL_EXSTYLE, (WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));
	DwmExtendFrameIntoClientArea(overlayWindow, &margin);
	ShowWindow(overlayWindow, SW_SHOW);
	UpdateWindow(overlayWindow);

	while (true)
	{
		if (PeekMessageW(&message, overlayWindow, 0, 0, PM_REMOVE))
		{
			DispatchMessageW(&message);
			TranslateMessage(&message);
		}

		HWND hwnd_active = GetForegroundWindow();

		if (hwnd_active == Globals::hWnd)
		{
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(overlayWindow, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		Sleep(1);
	}
}

bool overlay::InitWindow()
{
	// create a window class
	std::string r1 = RandomString(16);
	std::string r2 = RandomString(16);

	std::wstring lClassName = std::wstring(r1.begin(), r1.end());
	std::wstring lWindowName = std::wstring(r2.begin(), r2.end());

	WNDCLASSEXW wClass;
	wClass.cbSize = sizeof(WNDCLASSEXW);
	wClass.style = 0;
	wClass.lpfnWndProc = WinProc;
	wClass.cbClsExtra = NULL;
	wClass.cbWndExtra = NULL;
	wClass.hInstance = nullptr;
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hbrBackground = nullptr;
	wClass.lpszMenuName = lWindowName.c_str();
	wClass.lpszClassName = lClassName.c_str();
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);

	if (!RegisterClassExW(&wClass))
		return false;

	std::thread msgThread(HandleMessages, lClassName, lWindowName);
	msgThread.detach();

	return true;
}

bool overlay::DirectXInit()
{
	// initialize directx
	while (!overlayWindow) {}

	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		return false;

	GetClientRect(Globals::hWnd, &rc);

	windowWH = { rc.right - rc.left, rc.bottom - rc.top };

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = overlayWindow;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = windowWH.x;
	p_Params.BackBufferHeight = windowWH.y;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlayWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
		return false;

	D3DXCreateLine(p_Device, &p_Line);

	D3DXCreateFont(p_Device, 50, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Helvetica", &pFont); // Create the font used for text rendering
	D3DXCreateFont(p_Device, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Helvetica", &pESPFont); // Create the font used for esp rendering

	if (FAILED(D3DXCreateTextureFromFileInMemory(p_Device, kskPng, sizeof(kskPng), &ksk)))
		return false;

	if (FAILED(D3DXCreateTextureFromFileInMemory(p_Device, unknownController, sizeof(unknownController), &unknownCtrl)))
		return false;

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();

	// add default font and merge fontawesome icons
	//io.Fonts->AddFontFromMemoryTTF(comfortaaRegular, sizeof(comfortaaRegular), 13.f);
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 12);

	// you can uncomment this if you want to load the icon font/bold font
	//static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	//ImFontConfig icons_config;
	//icons_config.MergeMode = true;
	//icons_config.PixelSnapH = true;
	//icons_config.GlyphOffset = ImVec2(0.f, 2.f);
	//io.Fonts->AddFontFromMemoryTTF(font_awesome_data, sizeof(font_awesome_data), 16.f, &icons_config, icons_ranges); // icons font

	//// add title font and merge fontawesome icons
	//ImFontConfig titleConfig;
	//titleConfig.GlyphOffset = ImVec2(0.f, -2.f);
	//titleFont = io.Fonts->AddFontFromMemoryTTF(comfortaaBold, sizeof(comfortaaBold), 19.f, &titleConfig); // bold font

	//io.Fonts->AddFontFromMemoryTTF(font_awesome_data, sizeof(font_awesome_data), 21.f, &icons_config, icons_ranges);

	//// add big font and merge fontawesome icons
	//bigFont = io.Fonts->AddFontFromMemoryTTF(comfortaaRegular, sizeof(comfortaaRegular), 16.f);

	//icons_config.GlyphOffset = ImVec2(0.f, 1.f);
	//io.Fonts->AddFontFromMemoryTTF(font_awesome_data, sizeof(font_awesome_data), 18.f, &icons_config, icons_ranges);

	ImGui_ImplWin32_Init(overlayWindow);
	ImGui_ImplDX9_Init(p_Device);

	return true;
}

void InputHandler() {
	for (int i = 0; i < 5; i++) ImGui::GetIO().MouseDown[i] = false;
	int button = -1;
	if (GetAsyncKeyState(VK_LBUTTON)) button = 0;
	if (button != -1) ImGui::GetIO().MouseDown[button] = true;
}

void DrawImGui()
{
	// pass input to imgui
	InputHandler();

	// create an imgui frame

	// set the right window flags

	if (GetAsyncKeyState(VK_INSERT))
	{
		menuActive = !menuActive;
		Sleep(100);
	}

	if (menuActive)
		SetWindowLongA(overlayWindow, GWL_EXSTYLE, (WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));
	else
		SetWindowLongA(overlayWindow, GWL_EXSTYLE, (WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));

	// MENU
	if (menuActive) {
		Menu::Render();
	}
}

void overlay::Render()
{
	static ImDrawList* drawList;

	while (!GetAsyncKeyState(VK_END))
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		DrawImGui();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin("##scene", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

		drawList = ImGui::GetWindowDrawList();

		p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.f, 0);
		p_Device->BeginScene();

		Scene::Render();

		drawList->PushClipRectFullScreen();
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		p_Device->EndScene();
		p_Device->PresentEx(0, 0, 0, 0, 0);
	}
}

void overlay::DirectXShutdown()
{
	// destroy everything
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	p_Object->Release();
	p_Device->Release();
	pFont->Release();
	pESPFont->Release();
	p_Line->Release();

	DestroyWindow(overlayWindow);
}