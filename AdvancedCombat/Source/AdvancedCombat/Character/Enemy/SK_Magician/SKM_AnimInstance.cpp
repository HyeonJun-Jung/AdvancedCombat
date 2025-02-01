// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/SK_Magician/SKM_AnimInstance.h"

void USKM_AnimInstance::AnimNotify_SpawnMagic_Hand()
{
	Delegate_SpawnMagic_Hand.Broadcast();
}

void USKM_AnimInstance::AnimNotify_SpawnMagic_Staff()
{
	Delegate_SpawnMagic_Staff.Broadcast();
}
