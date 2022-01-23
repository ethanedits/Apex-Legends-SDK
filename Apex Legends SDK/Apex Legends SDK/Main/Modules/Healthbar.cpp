#include "Modules.h"

void DrawHealthbars(float x, float y, float width, float height, int thickness, int health, int shieldHealth)
{
	DrawFilledRectangle(x - (thickness + 5), y, thickness, height, ARGB(255, 0, 255, 0));
	DrawFilledRectangle(x - (thickness + 5), y, thickness, height - ((height / 100) * health), ARGB(120, 255, 0, 0));
	DrawFilledRectangle(x - (thickness * 2 + 12), y, thickness, height, ARGB(255, 0, 239, 252));
	DrawFilledRectangle(x - (thickness * 2 + 12), y, thickness, height - ((height / 125) * shieldHealth), ARGB(0, 0, 0, 0));

	if (shieldHealth > 0)
		DrawBorderBox(x - (thickness * 2 + 12) - 1, y - 1, thickness + 1, height + 1, 1, ARGB(255, 0, 0, 0));
	DrawBorderBox(x - (thickness + 5) - 1, y - 1, thickness + 1, height + 1, 1, ARGB(255, 0, 0, 0));
}

void Modules::Healthbar(BaseEntity pEntity, LocalEntity pLocal, Matrix m, float distance)
{
	Vector3 entHead = pEntity.BonePosition(8); // get head position
	Vector3 w2sHead = entHead.ScreenPosition(m); if (w2sHead.z <= 0.f) return;

	Vector3 entPos = pEntity.Position();
	Vector3 w2sPos = w2sPos.ScreenPosition(m); if (w2sPos.z <= 0.f) return;

	float height = abs(abs(w2sHead.y) - abs(w2sPos.y));
	float width = height / 2.f;
	float middle = w2sPos.x - (width / 2.f);

	if (distance <= 8000)
		if (pEntity.iTeam() == pLocal.iTeam())
			return;
			
		DrawHealthbars(middle, w2sHead.y, width, height, 2, pEntity.iHealth(), pEntity.iShield());
}