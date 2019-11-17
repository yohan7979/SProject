#pragma once

UENUM()
enum class EAnimMontageType : uint8
{
	None,
	NormalAttack_A,
	NormalAttack_B,
	NormalAttack_C,
	NormalAttack_D,
	JumpAttack,
	Ability_One,
	Ability_Two,
	Ability_Three,
	Ability_Four,
	Death_A,
	Death_B,
	CancelMotion
};

UENUM()
enum class ESkillType : uint8
{
	None,
	Basic_A,
	Basic_B,
	Basic_C,
	Basic_D,
	One,
	Two,
	Three,
	Four
};

UENUM()
enum class EWeaponCollisionType : uint8
{
	None,
	Melee,
	Round,
	ETC
};

UENUM()
enum class EDamageType : uint8
{
	Normal,
	Point,
	Radial
};