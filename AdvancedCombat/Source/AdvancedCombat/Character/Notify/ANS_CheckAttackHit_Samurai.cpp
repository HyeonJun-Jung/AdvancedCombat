// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_CheckAttackHit_Samurai.h"
#include "Character/Player/CombatPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Perception/AISense_Damage.h"
#include "Character/DamageType/DamageTypes.h"

void UANS_CheckAttackHit_Samurai::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{

}

void UANS_CheckAttackHit_Samurai::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    TArray<FHitResult> hitResults;
    FVector Start = MeshComp->GetSocketLocation(StartSocket);
    FVector End = MeshComp->GetSocketLocation(EndSocket);

    TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
    objectType.Emplace(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> ignore;
    ignore.Emplace(MeshComp->GetOwner());

    UKismetSystemLibrary::SphereTraceMultiForObjects(MeshComp, Start, End, 20, objectType,
        false, ignore, DebugType.GetValue(), hitResults, true);

    for (FHitResult hit : hitResults)
    {
        AActor* DamagedActor = hit.GetActor();
        ACombatPlayerCharacter* Player = Cast<ACombatPlayerCharacter>(DamagedActor);

        if (DamagedActor && !m_DamagedActors.Contains(DamagedActor))
        {
            m_DamagedActors.Add(DamagedActor);

            AActor* actor = MeshComp->GetOwner();
            if (actor)
            {
                UGameplayStatics::ApplyPointDamage(DamagedActor, 10, hit.ImpactNormal, hit,
                    actor->GetInstigatorController(), actor, GetDamageClass());
            }

            UAISense_Damage::ReportDamageEvent(DamagedActor, DamagedActor, actor,
                10, DamagedActor->GetActorLocation(), hit.ImpactPoint);
        }
    }
}

void UANS_CheckAttackHit_Samurai::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    m_DamagedActors.Empty();
}

TSubclassOf<UDamageType> UANS_CheckAttackHit_Samurai::GetDamageClass()
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
    default:
        break;
    }
    return UDamageType::StaticClass();
}
