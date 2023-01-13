// Fill out your copyright notice in the Description page of Project Settings.


#include "SPGameModeBase.h"
#include "Player/SPBaseCharacter.h"
#include "Player/SPPlayerController.h"
#include "UI/SPGameHUD.h"

ASPGameModeBase::ASPGameModeBase()
{
    DefaultPawnClass = ASPBaseCharacter::StaticClass();
    PlayerControllerClass = ASPPlayerController::StaticClass();

    HUDClass = ASPGameHUD::StaticClass();
}
