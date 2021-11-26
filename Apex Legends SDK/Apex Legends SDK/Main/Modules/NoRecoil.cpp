#include "Modules.h"

Vector3 oldPunch = { 0.f, 0.f, 0.f };

void Modules::NoRecoil(LocalEntity pLocal) {

	if (GetAsyncKeyState(VK_LBUTTON)) {
		Vector3 viewAngles = pLocal.getViewAngles();
		Vector3 punchAngle = pLocal.getAimPunch();

		Vector3 newAngle = viewAngles + (oldPunch - punchAngle);

		newAngle.Normalize();
		pLocal.setViewAngles(Vector2(newAngle.x, newAngle.y));

		oldPunch = punchAngle;
	}
}