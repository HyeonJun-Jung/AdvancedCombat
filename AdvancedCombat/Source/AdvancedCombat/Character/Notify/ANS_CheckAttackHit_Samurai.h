// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ACEnums.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ANS_CheckAttackHit_Samurai.generated.h"

UCLASS()
class ADVANCEDCOMBAT_API UANS_CheckAttackHit_Samurai : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

public:
	UPROPERTY(EditAnywhere, Category = Damage)
	FName StartSocket;

	UPROPERTY(EditAnywhere, Category = Damage)
	FName EndSocket;

	UPROPERTY(EditAnywhere, Category = Damage)
	float Radius = 30.f;

	UPROPERTY(EditAnywhere, Category = Damage)
	TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::None;

	UPROPERTY(EditAnywhere, Category = Damage)
	EDamageType DamageType = EDamageType::GuardableDamage;

	UPROPERTY(EditAnywhere, Category = Damage)
	float Damage = 10.f;

	TArray<AActor*> m_DamagedActors;

public:
	TSubclassOf<UDamageType> GetDamageClass();
};
