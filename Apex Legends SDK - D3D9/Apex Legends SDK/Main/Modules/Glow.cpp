#include "Modules.h"

void Modules::Glow(BaseEntity pEntity)
{
	pEntity.glowStyle = { 101,101,46,90 }; //Bright Glow, default is just outline
	pEntity.Glow(255.f, 0.f, 0.f);
}