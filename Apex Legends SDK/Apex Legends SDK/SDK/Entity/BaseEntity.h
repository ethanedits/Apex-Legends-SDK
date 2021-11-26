#pragma once
#include "../../Utilities/includes.h"
#include "LocalEntity.h"

class BaseEntity {
private:
	DWORD64 EntityList;
	DWORD64 BaseEnt;
	DWORD64 EntityHandle;

	std::string Identifier;

	DWORD64 atIndex(int index)
	{
		return Driver::rpm<DWORD64>(EntityList + (index << 5)); //Returns Entity Address
	}

public:

	DWORD64 Entity;
	LPCSTR IdentifierC = Identifier.c_str();
	glowMode glowStyle = { 0, 102, 100, 255 }; //Default Glow is Outline

	explicit BaseEntity(int index)
	{
		EntityList = Globals::modBase + OFFSET_ENTITYLIST;
		DWORD64 BaseEnt = Driver::rpm<DWORD64>(EntityList);
		Entity = atIndex(index);
		if (Entity != 0)
			EntityHandle = Driver::rpm<DWORD64>(Entity + 0x589);
			Identifier = Driver::rpm<std::string>(EntityHandle);
	}

	bool isPlayer()
	{
		return (bool)strcmp(IdentifierC, "player");
	}

	Vector3 Position() { 
		return Driver::rpm<Vector3>(Entity + OFFSET_ORIGIN);
	}

	Vector3 BonePosition(int boneIndex)
	{
		Bone bone = {};
		Vector3 vec_bone = Vector3();
		Vector3 pos = Position();

		ULONG64 bone_array = Driver::rpm<ULONG64>(Entity + OFFSET_BONES);
		ULONG64 bone_location = (boneIndex * 0x30);

		bone = Driver::rpm<Bone>(bone_array + bone_location);
		vec_bone.x = bone.x + pos.x;
		vec_bone.y = bone.y + pos.y;
		vec_bone.z = bone.z + pos.z;
		return vec_bone;
	}

	int iHealth() {
		return Driver::rpm<int>(Entity + OFFSET_HEALTH);
	}

	int iShield() {
		return Driver::rpm<int>(Entity + OFFSET_SHIELD);
	}

	int iTeam() {
		return Driver::rpm<int>(Entity + OFFSET_TEAM);
	}

	int isKnocked() {
		return Driver::rpm<int>(Entity + OFFSET_BLEED_OUT_STATE);
	}

	float distanceToLocal(LocalEntity pLocal) {
		return Position().distance(pLocal.Position());
	}

	void Glow(float r, float g, float b)
	{
		if (r == 0.f && g == 0.f && b == 0.f) //Glow Off
		{
			float time = 5000.f;
			glowMode mode = { 0, 0, 0, 0 };
			colorRGB color = { 255, 0, 0 };
			Driver::wpm<glowMode>(Entity + GLOW_TYPE, mode);
			Driver::wpm<colorRGB>(Entity + GLOW_COLOR, color);
			Driver::wpm<float>(Entity + GLOW_DISTANCE, 40000.f);
			Driver::wpm<float>(Entity + GLOW_LIFE_TIME, time);
			time -= 1.f;
			Driver::wpm<int>(Entity + GLOW_CONTEXT, 1);
			Driver::wpm<int>(Entity + GLOW_VISIBLE_TYPE, 1);
			Driver::wpm<glowFade>(Entity + GLOW_FADE, { 872415232, 872415232, time, time, time, time });
			return;
		}

		float time = 5000.f;

		colorRGB color = { r, g, b };
		Driver::wpm<glowMode>(Entity + GLOW_TYPE, glowStyle);
		Driver::wpm<colorRGB>(Entity + GLOW_COLOR, color);
		Driver::wpm<float>(Entity + GLOW_DISTANCE, 40000.f);
		Driver::wpm<float>(Entity + GLOW_LIFE_TIME, time);
		time -= 1.f;
		Driver::wpm<int>(Entity + GLOW_CONTEXT, 1);
		Driver::wpm<int>(Entity + GLOW_VISIBLE_TYPE, 1);
		Driver::wpm<glowFade>(Entity + GLOW_FADE, { 872415232, 872415232, time, time, time, time });
	}

	void PredictPosition(LocalEntity pLocal, Vector3* bonePos)
	{
		uintptr_t actWeaponID = Driver::rpm<uintptr_t>(pLocal.Entity + OFFSET_CURRENT_WEAPON) & 0xFFFF;
		uintptr_t currentWeapon = Driver::rpm<uintptr_t>(EntityList + (actWeaponID << 5));

		if (currentWeapon != 0)
		{
			float bulletSpeed = Driver::rpm<float>(currentWeapon + OFFSET_BULLET_SPEED);
			float bulletGravity = Driver::rpm<float>(currentWeapon + OFFSET_BULLET_GRAVITY);

			if (bulletSpeed > 1.f)
			{
				Vector3 muzzle = Driver::rpm<Vector3>(pLocal.Entity + OFFSET_CAMERAPOS);
				float time = bonePos->DistTo(muzzle) / bulletSpeed;
				bonePos->z += (700.f * bulletGravity * 0.5f) * (time * time);
				Vector3 velDelta = (Driver::rpm<Vector3>(Entity + OFFSET_ORIGIN - 0xC) * time);
				bonePos->x += velDelta.x;
				bonePos->y += velDelta.y;
				bonePos->z += velDelta.z;
			}
		}
	}

	void setGlowStyle(int GeneralGlowMode, int BorderGlowMode, int BorderSize, int TransparentLevel) //Should only be used for python userscripts
	{
		glowStyle = { (unsigned char)GeneralGlowMode, (unsigned char)BorderGlowMode, (unsigned char)BorderSize, (unsigned char)TransparentLevel };
	}
};