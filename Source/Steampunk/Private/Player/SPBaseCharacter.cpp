// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SPCharacterMovementComponent.h"
#include "Components/SPWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"


DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

ASPBaseCharacter::ASPBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USPHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());

    WeaponComponent = CreateDefaultSubobject<USPWeaponComponent>("WeaponComponent");
}

void ASPBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());
    check(GetMesh());

    OnHealthChanged(HealthComponent->GetHealth(), 0.f);
    HealthComponent->OnDeath.AddUObject(this, &ASPBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASPBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASPBaseCharacter::OnGroundLanded);
    
}

void ASPBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASPBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ASPBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASPBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASPBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASPBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASPBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &ASPBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed,this, &ASPBaseCharacter::OnStartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released,this, &ASPBaseCharacter::OnStopSprint);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USPWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USPWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USPWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USPWeaponComponent::Reload);
}


void ASPBaseCharacter::MoveForward(float Amount)
{
    IsMovimgForward = Amount > 0.f;
    if(Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASPBaseCharacter::MoveRight(float Amount)
{
    if(Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASPBaseCharacter::OnStartSprint()
{
    WantsToRun = true;
}

void ASPBaseCharacter::OnStopSprint()
{
    WantsToRun = false;
}


bool ASPBaseCharacter::IsRunning() const
{
    return  WantsToRun && IsMovimgForward && !GetVelocity().IsZero();
}

float ASPBaseCharacter::GetMovementDirection() const
{
    if(GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees: Degrees * FMath::Sign(CrossProduct.Z);
}

void ASPBaseCharacter::OnDeath()
{
    // PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.0f);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->StopFire();

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void ASPBaseCharacter::OnGroundLanded(const FHitResult &Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;

    if (FallVelocityZ < LandedDamageVelocity.X) return;

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

