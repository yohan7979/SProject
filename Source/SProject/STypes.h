#pragma once

UENUM()
enum class EAnimMontageType : uint8
{
	EAMT_None,
	EAMT_NormalAttack_A,
	EAMT_NormalAttack_B,
	EAMT_NormalAttack_C,
	EAMT_JumpAttack,
	EAMT_Ability_One,
	EAMT_Ability_Two,
	EAMT_Ability_Three,
	EAMT_Ability_Four,
	EAMT_Death_A,
	EAMT_Death_B,
	EAMT_CancelMotion
};

UENUM()
enum class ESkillType : uint8
{
	EAST_None,
	EAST_Basic_A,
	EAST_Basic_B,
	EAST_Basic_C,
	EAST_One,
	EAST_Two,
	EAST_Three,
	EAST_Four
};

UENUM()
enum class EWeaponCollisionType : uint8
{
	EWCT_None,
	EWCT_Melee,
	EWCT_Round,
	EWCT_ETC
};