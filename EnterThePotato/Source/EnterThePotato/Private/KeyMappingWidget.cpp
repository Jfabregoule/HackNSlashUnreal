// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyMappingWidget.h"
#include "../EnterThePotatoPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InputCoreTypes.h"


bool UKeyMappingWidget::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    // Trouver les boutons par leur nom

    if (Button_0)
        Button_0->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnFirstButtonClicked);

    if (Button_1)
        Button_1->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnSecondButtonClicked);

    if (Button_2)
        Button_2->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnThirdButtonClicked);

    if (Button_3)
        Button_3->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnFourthButtonClicked);

    if (Button_4)
        Button_4->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnFifthButtonClicked);

    if (Button_5)
        Button_5->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnSixthButtonClicked);

    if (Button_6)
        Button_6->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnSeventhButtonClicked);

    if (Button_7)
        Button_7->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnEightthButtonClicked);

    if (Button_8)
        Button_8->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnNinethButtonClicked);

    if (Button_9)
        Button_9->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnTenthButtonClicked);

    if (Button_10)
        Button_10->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnEleventhButtonClicked);

    return true;
}

void UKeyMappingWidget::StartRebinding(UInputAction* Action)
{
    if (!Action) return;

    ActionToRebind = Action;

    // Show a message on the UI asking for input
    UE_LOG(LogTemp, Warning, TEXT("Press any key to bind to action: %s"), *Action->GetFName().ToString());

    // Enable input listening
    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
        PC->InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &UKeyMappingWidget::OnKeySelected);
    }
}

void UKeyMappingWidget::OnKeySelected(FKey NewKey)
{
    if (!ActionToRebind) return;

    // Get the player controller
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    if (PlayerController)
    {
        PlayerController->RebindKey(ActionToRebind, NewKey);
        UE_LOG(LogTemp, Warning, TEXT("Rebound action: %s to %s"), *ActionToRebind->GetFName().ToString(), *NewKey.ToString());
    }

    // Reset UI state
    ActionToRebind = nullptr;

    // Restore input mode
    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
}

void UKeyMappingWidget::OnFirstButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->SetDestinationKeyboardAction);
}

void UKeyMappingWidget::OnSecondButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->SetDestinationKeyboardAction);
}

void UKeyMappingWidget::OnThirdButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->SetDestinationKeyboardAction);
}

void UKeyMappingWidget::OnFourthButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->SetDestinationKeyboardAction);
}

void UKeyMappingWidget::OnFifthButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->SetDestinationSprint);
}

void UKeyMappingWidget::OnSixthButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->SetDestinationZoom);
}

void UKeyMappingWidget::OnSeventhButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->SetDestinationUnZoom);
}

void UKeyMappingWidget::OnEightthButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->SetDestinationPause);
}

void UKeyMappingWidget::OnNinethButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->ThrowAction);
}

void UKeyMappingWidget::OnTenthButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->PickUpAction);
}

void UKeyMappingWidget::OnEleventhButtonClicked()
{
    AEnterThePotatoPlayerController* PlayerController = Cast<AEnterThePotatoPlayerController>(GetOwningPlayer());
    StartRebinding(PlayerController->HitAction);
}
