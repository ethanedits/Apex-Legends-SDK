#include "Modules.h"

void Modules::Glow(BaseEntity pEntity, LocalEntity pLocal)
{
	//pEntity.glowStyle = { 101,101,46,90 }; //Bright Glow, default is just outline

	int health = pEntity.iHealth();
	int bleedoutstate = pEntity.isKnocked();

	if (pEntity.iTeam() == pLocal.iTeam()) // DON'T GLOW ALLIES
	return;

	if(health <= 300 && health > 180)
	{
		pEntity.Glow(255.f, 255.f, 255.f); //white
	}
	else if (health <= 179 && health >  160)
	{
		pEntity.Glow(0.f, 255.f, 0.f); //green
	}
	else if (health <= 159 && health > 140)
	{
		pEntity.Glow(255.f, 255.f, 0.f); //yellow
	}
	else if (health <= 139 && health > 110)
	{
		pEntity.Glow(255.f, 0.f, 0.f); //red
	}
	else if (health <= 109 && health > 70)
	{
		pEntity.Glow(255.f, 0.f, 255.f); //pink
	}
	else if(health <= 69 && health > 40)
	{
		pEntity.Glow(0.f, 0.f, 255.f); //blue
	}
	else if (health < 40)
	{
		pEntity.Glow(0.f, 255.f, 255.f); //cyan
	}
	else if (bleedoutstate != 0)
	{
		pEntity.Glow(136.f, 0.f, 255.f); //purple
	}
}