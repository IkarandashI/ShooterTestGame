// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SPAICharacter.h"
#include "AI/SPAIController.h"
#include "Components/SPAIWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"


ASPAICharacter::ASPAICharacter(const FObjectInitializer &ObjInit)
  :Super(ObjInit.SetDefaultSubobjectClass<USPAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASPAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if(GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.f,200.f,0.f);
    }
}

void ASPAICharacter::OnDeath()
{
    Super::OnDeath();

    const auto SPController = Cast<AAIController>(Controller);
    if (SPController && SPController->BrainComponent)
    {
        SPController->BrainComponent->Cleanup();
    }
}
