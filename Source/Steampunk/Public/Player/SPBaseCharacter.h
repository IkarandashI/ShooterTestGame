// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPBaseWeapon.h"
#include "Components/SPHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "SPBaseCharacter.generated.h"

class UCameraComponent;
class USPHealthComponent;
class UTextRenderComponent;
class USPWeaponComponent;

UCLASS()
class STEAMPUNK_API ASPBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASPBaseCharacter(const FObjectInitializer& ObjInit);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USPHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USPWeaponComponent* WeaponComponent;

    UPROPERTY(EditAnywhere, Category="Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditAnywhere, Category="Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditAnywhere, Category="Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditAnywhere, Category="Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    virtual void OnDeath();
    
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category="Movement")
    bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category="Movement")
    float GetMovementDirection() const; 
        
private:
    bool WantsToRun = false;
    bool IsMovimgForward = false;
    
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    
    void OnStartSprint();
    void OnStopSprint();

    
    void OnHealthChanged(float Health, float HealthDelta);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};


