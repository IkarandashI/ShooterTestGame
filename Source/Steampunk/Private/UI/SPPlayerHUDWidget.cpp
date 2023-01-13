// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPPlayerHUDWidget.h"
#include "Components/SPHealthComponent.h"
#include "Components/SPWeaponComponent.h"
#include "SPUtils.h"

bool USPPlayerHUDWidget::Initialize()
{
    const auto HealthComponent = SPUtils::GetSPUPlayerComponent<USPHealthComponent>(GetOwningPlayerPawn());
    if(HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USPPlayerHUDWidget::OnHealthChanged);
    }
    
    return Super::Initialize();
}

void USPPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        OnTakeDamage();
    }
    
}

float USPPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = SPUtils::GetSPUPlayerComponent<USPHealthComponent>(GetOwningPlayerPawn());
    if(!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USPPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = SPUtils::GetSPUPlayerComponent<USPWeaponComponent>(GetOwningPlayerPawn());
    if(!WeaponComponent) return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

bool USPPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = SPUtils::GetSPUPlayerComponent<USPWeaponComponent>(GetOwningPlayerPawn());
    if(!WeaponComponent) return false;

    return WeaponComponent->GetWeaponAmmoData(AmmoData);
}

bool USPPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = SPUtils::GetSPUPlayerComponent<USPHealthComponent>(GetOwningPlayerPawn());
    return  HealthComponent && !HealthComponent->IsDead();
}

bool USPPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return  Controller && Controller->GetStateName() == NAME_Spectating;
}


