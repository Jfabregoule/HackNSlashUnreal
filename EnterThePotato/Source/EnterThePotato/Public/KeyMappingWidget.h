// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputAction.h"
#include "KeyMappingWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENTERTHEPOTATO_API UKeyMappingWidget : public UUserWidget
{
	GENERATED_BODY()
	
    public:
        // UI Button Clicked: Start Rebinding
        UFUNCTION(BlueprintCallable)
        void StartRebinding(UInputAction* Action);

    protected:
        virtual bool Initialize() override;

    private:
        UPROPERTY(meta = (BindWidget))
        class UButton* Button_0;

        UPROPERTY(meta = (BindWidget))
        class UButton* Button_1;

        UPROPERTY(meta = (BindWidget))
        class UButton* Button_2;

        UPROPERTY(meta = (BindWidget))
        class UButton* Button_3;

        UPROPERTY(meta = (BindWidget))
        class UButton* Button_4;

        UPROPERTY(meta = (BindWidget))
        class UButton* Button_5;

        UPROPERTY(meta = (BindWidget))
        class UButton* Button_6;

        UPROPERTY(meta = (BindWidget))
        class UButton* Button_7;

        UPROPERTY(meta = (BindWidget))
        class UButton* Button_8;

        UPROPERTY(meta = (BindWidget))
        class UButton* Button_9;

        UPROPERTY(meta = (BindWidget))
        class UButton* Button_10;

        UPROPERTY()
        UInputAction* ActionToRebind;

        // Function to handle key press event
        void OnKeySelected(FKey NewKey);
        void OnFirstButtonClicked();
        void OnSecondButtonClicked();
        void OnThirdButtonClicked();
        void OnFourthButtonClicked();
        void OnFifthButtonClicked();
        void OnSixthButtonClicked();
        void OnSeventhButtonClicked();
        void OnEightthButtonClicked();
        void OnNinethButtonClicked();
        void OnTenthButtonClicked();
        void OnEleventhButtonClicked();
};
