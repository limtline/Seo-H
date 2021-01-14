// Copyright Epic Games, Inc. All Rights Reserved.


#include "BossModeGameModeBase.h"
#include "BMHUD.h"

ABossModeGameModeBase::ABossModeGameModeBase() : Super()
{
	// 블루프린트 캐릭터를 기본 폰 클래스로 설정하기
	static ConstructorHelpers::FClassFinder<APawn>PlayerPawnClassFinder(TEXT("/Game/FPCharacter"));

	DefaultPawnClass = PlayerPawnClassFinder.Class;

	HUDClass = ABMHUD::StaticClass();
}