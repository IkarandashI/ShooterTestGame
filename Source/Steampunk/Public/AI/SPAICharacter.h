// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SPBaseCharacter.h"
#include "SPAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class STEAMPUNK_API ASPAICharacter : public ASPBaseCharacter
{
	GENERATED_BODY()

public:
    ASPAICharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
    UBehaviorTree* BehaviorTreeAsset;
    
protected:
    virtual void OnDeath() override;
};
