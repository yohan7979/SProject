#pragma once

UENUM()
enum class EAnimMontageType : uint8
{
	EAMT_None,
	EAMT_NormalAttack_A,
	EAMT_NormalAttack_B,
	EAMT_NormalAttack_C,
	EAMT_Ability_One,
	EAMT_Death_A,
	EAMT_Death_B
};

UENUM()
enum class ESkillType : uint8
{
	EAST_None,
	EAST_One,
	EAST_Two,
	EAST_Three,
	EAST_Four
};

UENUM()
enum class EWeaponCollisionType : uint8
{
	EWCT_None,
	EWCT_Left,
	EWCT_Right,
	EWCT_ETC
};