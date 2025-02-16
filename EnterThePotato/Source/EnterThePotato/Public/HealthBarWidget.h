// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENTERTHEPOTATO_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Function to update health bar UI
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

protected:
	// Progress bar reference
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
};
