// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "Widget_Status.generated.h"

class UProgressBar;

UCLASS()
class ADVANCEDCOMBAT_API UWidget_Status : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdateStatusBars();

public:
	virtual void HealthUpdated(const FOnAttributeChangeData& AttributeChangeData);
	virtual void MaxHealthUpdated(const FOnAttributeChangeData& AttributeChangeData);
	virtual void ManaUpdated(const FOnAttributeChangeData& AttributeChangeData);
	virtual void MaxManaUpdated(const FOnAttributeChangeData& AttributeChangeData);
	virtual void StaminaUpdated(const FOnAttributeChangeData& AttributeChangeData);
	virtual void MaxStaminaUpdated(const FOnAttributeChangeData& AttributeChangeData);

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UProgressBar* ManaBar;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, EditAnywhere)
	UProgressBar* StaminaBar;

private:
	float MaxHealth;
	float CurrentHealth;
	float MaxMana;
	float CurrentMana;
	float MaxStamina;
	float CurrentStamina;
};
