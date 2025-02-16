// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "EPGameInstance.h"
#include "EnterThePotato/EnterThePotatoPlayerController.h"

bool UPauseMenuWidget::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    // Trouver les boutons par leur nom

    if (ResumeButton)
        ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);

    if (ReturnButton)
        ReturnButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnReturnClicked);

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);

    return true;
}

void UPauseMenuWidget::OnResumeClicked()
{
    AEnterThePotatoPlayerController* PC = AEnterThePotatoPlayerController::getController(this);
    if (PC && PC->getPauseMenuInstance())
    {
        PC->getPauseMenuInstance()->RemoveFromParent();
        PC->setPauseMenuInstanceNULL();
    }


    UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UPauseMenuWidget::OnReturnClicked()
{
    UEPGameInstance* gameInstance = Cast<UEPGameInstance>(GetGameInstance());
    UE_LOG(LogTemp, Warning, TEXT("Bouton Jouer cliqué !"));
    UWorld* World = GetWorld();
    if (World && gameInstance)
    {
        gameInstance->LoadMenuLevel(World);
    }
}

void UPauseMenuWidget::OnQuitClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Bouton Quitter cliqué !"));
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}
