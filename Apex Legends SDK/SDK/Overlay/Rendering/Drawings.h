#pragma once
#include "overlay.h"

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

void DrawString(const char* String, int x, int y, D3DCOLOR color, ID3DXFont* font, DWORD format = DT_NOCLIP); // there is also DT_CENTER, DT_BOTTOM and so on
void DrawShadowString(const char* String, int x, int y, int offset, D3DCOLOR color, D3DCOLOR shadowColor, ID3DXFont* font, DWORD format = DT_NOCLIP);
void DrawOutlinedString(const char* str, int x, int y, D3DCOLOR color, ID3DXFont* font, ID3DXFont* outlineFont, D3DCOLOR outlineColor = D3DCOLOR_ARGB(255, 0, 0, 0), DWORD format = DT_NOCLIP);
void DrawFilledRectangle(int x, int y, int w, int h, D3DCOLOR color);
void DrawBorderBox(int x, int y, int x2, int y2, int thickness, D3DCOLOR color);
void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
void DrawCircle(int x, int y, float radius, float width, float rez, D3DCOLOR color);