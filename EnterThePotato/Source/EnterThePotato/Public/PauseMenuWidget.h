// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENTERTHEPOTATO_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual bool Initialize() override;

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ReturnButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;

   

    UFUNCTION()
    void OnResumeClicked();

    UFUNCTION()
    void OnReturnClicked();

    UFUNCTION()
    void OnQuitClicked();
    private:
        UUserWidget* PauseMenuInstance;
};
