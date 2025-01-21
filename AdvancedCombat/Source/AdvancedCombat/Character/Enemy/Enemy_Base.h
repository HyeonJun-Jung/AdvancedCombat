// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Character_Base.h"
#include "GameplayEffectTypes.h"
#include "Enemy_Base.generated.h"

UCLASS()
class ADVANCEDCOMBAT_API AEnemy_Base : public ACharacter_Base
{
	GENERATED_BODY()
	
public:
	AEnemy_Base();

protected:
	virtual void BeginPlay() override;

/// <summary>
/// Damaged
/// </summary>

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void DealWithDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, class AActor* DamageCauser);
	void HealthChanged(const FOnAttributeChangeData& Data);

public:
	TObjectPtr<class APatrolRoute> GetPatrolRoute() { return patrolRoute; }

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Patrol")
	TObjectPtr<class APatrolRoute> patrolRoute;

private:
	void LimitMaxHeight(float maxHeight);
	void DisableGravity(float duration);

	UFUNCTION()
	void GetUpFromGround(const FHitResult& hit);

private:
	FTimerHandle handle_LimitMaxHeight;
	float maxHeight;

//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
//	TSubclassOf<class UGameplayEffect> InitStatusEffect;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
//	UCurveFloat* HealthCurve;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
//	UCurveFloat* DamageCurve;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS_PROJECT|UI")
//	TSubclassOf<class UWidget_FloatingStatusBar> UIFloatingStatusBarClass;
//
//	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GAS_PROJECT|UI")
//	class UWidgetComponent* UIFloatingStatusBarComponent;
//
//	UPROPERTY()
//	class UWidget_FloatingStatusBar* UIFloatingStatusBar;

	FDelegateHandle HealthChangedDelegateHandle;
};
