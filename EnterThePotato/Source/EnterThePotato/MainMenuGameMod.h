// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuGameMod.generated.h"

/**
 * 
 */
UCLASS()
class ENTERTHEPOTATO_API AMainMenuGameMod : public AGameModeBase
{
	GENERATED_BODY()

    public:
        virtual void BeginPlay() override;
        UUserWidget* getMainMenuInstance();
       
    protected:
        // Classe du Widget (modifiable dans l��diteur)
        UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<class UUserWidget> MainMenuWidgetClass;
        

    private:
        UPROPERTY()
        UUserWidget* MainMenuWidget;
        

};
