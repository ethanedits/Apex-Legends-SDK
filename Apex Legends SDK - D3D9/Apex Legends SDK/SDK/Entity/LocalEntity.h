#pragma once
#include "../../Utilities/includes.h"

class LocalEntity {
public:

	uintptr_t Entity;

	explicit LocalEntity()
	{
		Entity = Driver::rpm<uintptr_t>(Globals::modBase + OFFSET_LOCAL_ENT);
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

	Vector3 getViewAngles() {
		return Driver::rpm<Vector3>(Entity + OFFSET_VIEWANGLES);
	}

	void setViewAngles(Vector2 angles) {
		Driver::wpm<Vector2>(Entity + OFFSET_VIEWANGLES, { angles.x, angles.y });
	}

	Vector3 getAimPunch() {
		return Driver::rpm<Vector3>(Entity + OFFSET_AIMPUNCH);
	}

	Vector3 getBreathAngles() {
		return Driver::rpm<Vector3>(Entity + OFFSET_BREATH_ANGLES);
	}

	Vector3 getRecoil() {
		return *(Vector3*)(Entity + OFFSET_AIMPUNCH);
	}

	Vector3 getCamPosition() {
		return *(Vector3*)(Entity + OFFSET_CAMERAPOS);
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
};