#include "Drawings.h"
#define M_PI 3.14159265

void DrawString(const char* String, int x, int y, D3DCOLOR color, ID3DXFont* font, DWORD format)
{
	RECT FontPos;
	FontPos.left = x;
	FontPos.top = y;
	font->DrawTextA(0, String, strlen(String), &FontPos, format, color);
}

void DrawOutlinedString(const char* str, int x, int y, D3DCOLOR color, ID3DXFont* font, ID3DXFont* outlineFont, D3DCOLOR outlineColor, DWORD format)
{
	RECT fontPos;
	RECT outlinePos;
	fontPos.left = x;
	fontPos.top = y;
	outlinePos.left = x - 1;
	outlinePos.top = y - 1;

	outlineFont->DrawTextA(0, str, strlen(str), &outlinePos, format, outlineColor);
	font->DrawTextA(0, str, strlen(str), &fontPos, format, color);
}

void DrawShadowString(const char* String, int x, int y, int offset, D3DCOLOR color, D3DCOLOR shadowColor, ID3DXFont* font, DWORD format)
{
	RECT FontPos;
	RECT shadowPos;
	FontPos.left = x;
	FontPos.top = y;
	shadowPos.left = x - offset;
	shadowPos.top = y + offset;
	font->DrawTextA(0, String, strlen(String), &shadowPos, format, shadowColor);
	font->DrawTextA(0, String, strlen(String), &FontPos, format, color);
}

void DrawFilledRectangle(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT rect = { x, y, x + w, y + h };
	p_Device->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawBorderBox(int x, int y, int x2, int y2, int thickness, D3DCOLOR color)
{
	DrawFilledRectangle(x, y, x2, thickness, color); // top
	DrawFilledRectangle(x, y + y2, x2, thickness, color); // bottom
	DrawFilledRectangle(x, y, thickness, y2, color); // left
	DrawFilledRectangle(x + x2, y, thickness, y2 + thickness, color); // right
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
{
	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	p_Line->SetWidth(thickness);
	p_Line->Draw(Line, 2, color);
}

void DrawCircle(int x, int y, float radius, float width, float rez, D3DCOLOR color)
{
	float si, co, nsi, nco, sw;
	bool antialiasing = p_Line->GetAntialias();
	p_Line->SetAntialias(true);
	sw = p_Line->GetWidth();
	p_Line->SetWidth(width);
	D3DXVECTOR2 rect[2];

	float step = M_PI * 2.0 / rez;
	float dec = step / 50;

	for (float i = 0; i < M_PI * 2.0; i += step)
	{
		si = x + (sin(i - dec) * radius);
		co = y + (cos(i - dec) * radius);

		nsi = x + (sin(i + step + dec) * radius);
		nco = y + (cos(i + step + dec) * radius);

		rect[0].x = si;
		rect[0].y = co;

		rect[1].x = nsi;
		rect[1].y = nco;

		p_Line->Draw(rect, 2, color);
	}
	p_Line->SetAntialias(antialiasing);
	p_Line->SetWidth(sw);
}