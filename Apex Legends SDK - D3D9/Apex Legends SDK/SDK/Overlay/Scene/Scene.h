#pragma once

namespace Scene {
	void Render() {
		//Important Stuff
		LocalEntity pLocal;
		uint64_t viewRenderer = Driver::rpm<uint64_t>(Globals::modBase + OFFSET_RENDER);
		uint64_t viewMatrix = Driver::rpm<uint64_t>(viewRenderer + OFFSET_MATRIX);
		Matrix m = Driver::rpm<Matrix>(viewMatrix);

		//No-Recoil
		if (MenuSettings::noRecoilEnabled)
			Modules::NoRecoil(pLocal);

		//Aimbot Context Reset
		AimContext aimCtx; aimCtx.closestX = 50000; aimCtx.closestY = 50000; aimCtx.FOV = MenuSettings::aimbotFOV; aimCtx.aSmoothAmount = MenuSettings::aimbotSmoothness;
		
		//Aimbot FOV Circle
		if (MenuSettings::fovCircle)
			DrawCircle((windowWH.x / 2), (windowWH.y / 2), MenuSettings::aimbotFOV, 3, 100, 0xffffffff);

		//Entity Loop
		for (int i = 0; i < 100; i++)
		{	
			BaseEntity pEntity(i);
			if (pEntity.isPlayer() && pEntity.iHealth() != 0 && pEntity.Entity != pLocal.Entity)
			{
				float distance = pEntity.distanceToLocal(pLocal);

				//Glow ESP
				if (MenuSettings::glowEnabled)
					Modules::Glow(pEntity);

				//Aimbot
				if (MenuSettings::aimbotEnabled)
					if (distance <= 3500)
						Modules::Aimbot(pEntity, pLocal, m, &aimCtx, i);

				//Box ESP
				if (MenuSettings::espEnabled)
					Modules::ESP(pEntity, pLocal, m, distance);

				//Healthbar
				if (MenuSettings::healthbarEnabled)
					Modules::Healthbar(pEntity, m, distance);
			}
		}

		//Aimbot
		if (MenuSettings::aimbotEnabled)
			Modules::AimbotMove(&aimCtx);
	}
}