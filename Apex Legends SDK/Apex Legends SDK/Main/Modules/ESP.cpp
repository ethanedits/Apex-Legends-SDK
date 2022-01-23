#include "Modules.h"

void Modules::ESP(BaseEntity pEntity, LocalEntity pLocal, Matrix m, float distance)
{
	Vector3 entHead = pEntity.BonePosition(8); // get head position
	Vector3 w2sHead = entHead.ScreenPosition(m); if (w2sHead.z <= 0.f) return;

	Vector3 entPos = pEntity.Position();
	Vector3 w2sPos = w2sPos.ScreenPosition(m); if (w2sPos.z <= 0.f) return;

	float height = abs(abs(w2sHead.y) - abs(w2sPos.y));
	float width = height / 2.f;
	float middle = w2sPos.x - (width / 2.f);

	if (distance <= 8000) {
		float boxThickness = 2;
		float snaplinesThickness = 2;
		int boxColor;

		if (pEntity.iTeam() == pLocal.iTeam())
		return;
			//If you want Team ESP uncomment the line below
			//boxColor = 0xff42cbf5; //Hex Color (TEAM)
		else
			boxColor = 0xfffc0303; //Hex Color (ENEMY)

		//Main Box
		DrawBorderBox(middle, w2sHead.y, width, height, boxThickness, boxColor);


		if (distance <= 3000) {
			//Outside Outline
			DrawBorderBox(middle - boxThickness, w2sHead.y - boxThickness, width + (boxThickness * 2), height + (boxThickness * 2), boxThickness, 0xff000000);

			//Inside Outline
			DrawBorderBox(middle + boxThickness, w2sHead.y + boxThickness, width - (boxThickness * 2), height - (boxThickness * 2), boxThickness, 0xff000000);

			//Snaplines
			if (MenuSettings::snaplinesEnabled)
				DrawLine(windowWH.x / 2, windowWH.y, w2sHead.x, w2sHead.y, boxThickness, boxColor);
		}
	}
}