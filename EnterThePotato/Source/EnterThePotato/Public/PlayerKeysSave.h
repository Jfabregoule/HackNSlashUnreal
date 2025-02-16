// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerKeysSave.generated.h"

/**
 * 
 */
UCLASS()
class ENTERTHEPOTATO_API UPlayerKeysSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TMap<FString, FKey> SavedKeyBindings;
};
