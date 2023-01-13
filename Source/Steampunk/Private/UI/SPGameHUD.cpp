// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

void ASPGameHUD::DrawHUD()
{
    Super::DrawHUD();

    //DrawCrossHair();
}

void ASPGameHUD::BeginPlay()
{
    Super::BeginPlay();
    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if(PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }
}

void ASPGameHUD::DrawCrossHair()
{
    const TInterval<float> Centre(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;
    
    DrawLine(Centre.Min - HalfLineSize, Centre.Max, Centre.Min + HalfLineSize, Centre.Max, LineColor, LineThickness);
    DrawLine(Centre.Min, Centre.Max - HalfLineSize, Centre.Min, Centre.Max + HalfLineSize, LineColor, LineThickness);
}
