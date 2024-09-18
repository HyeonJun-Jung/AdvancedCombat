// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/Samurai/Samurai_AnimInstance.h"

void USamurai_AnimInstance::AnimNotify_SpawnSlash()
{
	Delegate_SpawnSlash.Broadcast();
}

void USamurai_AnimInstance::AnimNotify_SpawnIdleSlash()
{
	Delegate_SpawnIdleSlash.Broadcast();
}
