#include "Modules.h"

void Modules::Aimbot(BaseEntity pEntity, LocalEntity pLocal, Matrix m, AimContext* Ctx, int index) {

	Vector3 entAimPos = pEntity.BonePosition(7); //0 Hips //5 Body //7 Neck //8 Head

	pEntity.PredictPosition(pLocal, &entAimPos);

	Vector3 w2sEntAimPos = entAimPos.ScreenPosition(m); if (w2sEntAimPos.z <= 0.f) return;

	int entX = w2sEntAimPos.x;
	int entY = w2sEntAimPos.y;

	Ctx->entNewVisTime = Driver::rpm<float>(pEntity.Entity + OFFSET_VISIBLE_TIME);
	int entKnockedState = pEntity.isKnocked();
	int playerTeamID = pLocal.iTeam();
	int entTeamID = pEntity.iTeam();

	if (entTeamID != playerTeamID) {

		if (Ctx->entNewVisTime != Ctx->entOldVisTime[index])
		{
			Ctx->visCooldownTime[index] = 24;

			if (entKnockedState == 0)
			{

				if (abs(Ctx->crosshairX - entX) < abs(Ctx->crosshairX - Ctx->closestX) && abs(Ctx->crosshairX - entX) < Ctx->FOV && abs(Ctx->crosshairY - entY) < abs(Ctx->crosshairY - Ctx->closestY) && abs(Ctx->crosshairY - entY) < Ctx->FOV)
				{
					Ctx->closestX = entX;
					Ctx->closestY = entY;
				}
			}
			Ctx->entOldVisTime[index] = Ctx->entNewVisTime;
		}
		if (Ctx->visCooldownTime[index] >= 0) Ctx->visCooldownTime[index] -= 1;
	}
}

void Modules::AimbotMove(AimContext* Ctx) {

	int aimKey = 0;
	switch (MenuSettings::aimbotKeyID) {
	case 0: // "Right Mouse"
		aimKey = VK_RBUTTON;
		break;
	case 1: // "Left Alt"
		aimKey = VK_LMENU;
		break;
	case 2: // "Left Mouse"
		aimKey = VK_LBUTTON;
		break;
	case 3: // "Left Shift"
		aimKey = VK_LSHIFT;
		break;
	case 4: // "Space"
		aimKey = VK_SPACE;
		break;
	}

	if (Ctx->closestX != 50000 && Ctx->closestY != 50000)
	{
		if (GetAsyncKeyState(aimKey))
		{
			Ctx->aX = (Ctx->closestX - Ctx->crosshairX) / Ctx->aSmoothAmount;
			Ctx->aY = (Ctx->closestY - Ctx->crosshairY) / Ctx->aSmoothAmount;

			CURSORINFO ci = { sizeof(CURSORINFO) };
			if (GetCursorInfo(&ci))
			{
				if (ci.flags == 0)
					mouse_event(MOUSEEVENTF_MOVE, Ctx->aX, Ctx->aY, 0, 0);
			}
		}
	}
}
