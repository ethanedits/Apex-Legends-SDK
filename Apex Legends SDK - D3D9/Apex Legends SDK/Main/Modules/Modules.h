#pragma once
#include "../../SDK/Entity/BaseEntity.h"
#include "../../SDK/Overlay/Rendering/Overlay.h"

namespace Modules {
	void Aimbot(BaseEntity pEntity, LocalEntity pLocal, Matrix m, AimContext* Ctx, int index);
	void AimbotMove(AimContext* Ctx);
	void NoRecoil(LocalEntity pLocal);
	void Glow(BaseEntity pEntity);
	void ESP(BaseEntity pEntity, LocalEntity pLocal, Matrix m, float distance);
	void Healthbar(BaseEntity pEntity, Matrix m, float distance);
}