// Copyright Epic Games, Inc. All Rights Reserved.


#include "BossModeGameModeBase.h"
#include "BMHUD.h"

ABossModeGameModeBase::ABossModeGameModeBase() : Super()
{
	// �������Ʈ ĳ���͸� �⺻ �� Ŭ������ �����ϱ�
	static ConstructorHelpers::FClassFinder<APawn>PlayerPawnClassFinder(TEXT("/Game/FPCharacter"));

	DefaultPawnClass = PlayerPawnClassFinder.Class;

	HUDClass = ABMHUD::StaticClass();
}