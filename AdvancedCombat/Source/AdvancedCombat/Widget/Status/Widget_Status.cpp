// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Status/Widget_Status.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Player/AbililtyCombatPlayerCharacter.h"
#include "Components/ProgressBar.h"

void UWidget_Status::NativeConstruct()
{
	Super::NativeConstruct();

	AAbililtyCombatPlayerCharacter* player =
		Cast<AAbililtyCombatPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!player) return;

	player->BindStatusWidgetFunction(this);
	
	MaxHealth = player->GetMaxHealth(); CurrentHealth = player->GetHealth();
	MaxMana = player->GetMaxMana(); CurrentMana = player->GetMana();
	MaxStamina = player->GetMaxStamina(); CurrentHealth = player->GetStamina();
	UpdateStatusBars();
}

void UWidget_Status::UpdateStatusBars()
{
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
	ManaBar->SetPercent(CurrentMana / MaxMana);
	StaminaBar->SetPercent(CurrentStamina / MaxStamina);
}

void UWidget_Status::HealthUpdated(const FOnAttributeChangeData& AttributeChangeData)
{
	CurrentHealth = AttributeChangeData.NewValue;
	UpdateStatusBars();
}

void UWidget_Status::MaxHealthUpdated(const FOnAttributeChangeData& AttributeChangeData)
{
	MaxHealth = AttributeChangeData.NewValue;
	UpdateStatusBars();
}

void UWidget_Status::ManaUpdated(const FOnAttributeChangeData& AttributeChangeData)
{
	CurrentMana = AttributeChangeData.NewValue;
	UpdateStatusBars();
}

void UWidget_Status::MaxManaUpdated(const FOnAttributeChangeData& AttributeChangeData)
{
	MaxMana = AttributeChangeData.NewValue;
	UpdateStatusBars();
}

void UWidget_Status::StaminaUpdated(const FOnAttributeChangeData& AttributeChangeData)
{
	CurrentStamina = AttributeChangeData.NewValue;
	UpdateStatusBars();
}

void UWidget_Status::MaxStaminaUpdated(const FOnAttributeChangeData& AttributeChangeData)
{
	MaxStamina = AttributeChangeData.NewValue;
	UpdateStatusBars();
}
