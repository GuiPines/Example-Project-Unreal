// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatchSessionInfo.generated.h"

USTRUCT(BlueprintType)
struct FMatchSessionInfo 
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 Id = -1;

    UPROPERTY(BlueprintReadOnly)
    FString Name;

    UPROPERTY(BlueprintReadOnly)
    FString ServerIp;

    UPROPERTY(BlueprintReadOnly)
    int32 ServerPort = 0;
};
