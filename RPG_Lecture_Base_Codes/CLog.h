// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#define Print(text) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, text);
#define PrintTime(text, time) GEngine->AddOnScreenDebugMessage(-1, time, FColor::Blue, text);
#define PrintFormat(text, format) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT(text), format));

#include "CoreMinimal.h"

class U05_CPP_API CLog
{
public:
	CLog();
	~CLog();
};
