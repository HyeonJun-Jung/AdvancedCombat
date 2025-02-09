// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_GAS_CheckAttackHit.h"
#include "Character/Character_Base.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Perception/AISense_Damage.h"
#include "Character/DamageType/DamageTypes.h"
#include "AbilitySystemComponent.h"

void UANS_GAS_CheckAttackHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    OwningCharacter = Cast<ACharacter_Base>(MeshComp->GetOwner());
}

void UANS_GAS_CheckAttackHit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

    TArray<FHitResult> hitResults;
    FVector Start = MeshComp->GetSocketLocation(StartSocket);
    FVector End = MeshComp->GetSocketLocation(EndSocket);

    TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
    objectType.Emplace(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> ignore;
    ignore.Emplace(MeshComp->GetOwner());

    UKismetSystemLibrary::SphereTraceMultiForObjects(MeshComp, Start, End, Radius, objectType,
        false, ignore, DebugType.GetValue(), hitResults, true);

    if (!OwningCharacter || !DamageEffect) return;

    for (FHitResult hit : hitResults)
    {
        AActor* DamagedActor = hit.GetActor();
        ACharacter_Base* Target = Cast<ACharacter_Base>(DamagedActor);

        if (Target && !m_DamagedActors.Contains(Target))
        {
            m_DamagedActors.Add(Target);

            /*
                Apply Damage GameplayEffect To Target
            */
            UAbilitySystemComponent* SourceASC = OwningCharacter->GetAbilitySystemComponent();
            UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
            if (!SourceASC || !TargetASC) continue;

            FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
            EffectContext.AddSourceObject(this);
            EffectContext.AddHitResult(hit);

            FGameplayEffectSpecHandle NewHandle = SourceASC->MakeOutgoingSpec(DamageEffect, OwningCharacter->GetCharacterLevel(), EffectContext);
            if (NewHandle.IsValid())
            {
                FActiveGameplayEffectHandle ActiveGEHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TargetASC);
            }

            /*
                Report Damage Event For AI Sensing
            */
            UAISense_Damage::ReportDamageEvent(DamagedActor, DamagedActor, OwningCharacter,
                10, DamagedActor->GetActorLocation(), hit.ImpactPoint);
        }
    }
}

void UANS_GAS_CheckAttackHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    /*
        Clear Array For Next ANS
    */
    m_DamagedActors.Empty();
}

TSubclassOf<UDamageType> UANS_GAS_CheckAttackHit::GetDamageClass()
{
    switch (DamageType)
    {
    case EDamageType::NormalDamage:
        return UNormalDamage::StaticClass();
        break;
    case EDamageType::GuardableDamage:
        return  UGuardableDamage::StaticClass();
        break;
    case EDamageType::ParryableDamage:
        return UParryableDamage::StaticClass();
        break;
    case EDamageType::UnbreakableDamage:
        return UUnbreakableDamage::StaticClass();
        break;
    case EDamageType::StingDamage:
        return UStingDamage::StaticClass();
        break;
    case EDamageType::AirborneDamage:
        return UAirborneDamage::StaticClass();
        break;
    case EDamageType::InAirDamage:
        return UInAirDamage::StaticClass();
        break;
    case EDamageType::HitDownDamage:
        return UHitDownDamage::StaticClass();
    default:
        break;
    }
    return UDamageType::StaticClass();
}

