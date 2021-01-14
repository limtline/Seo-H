// Fill out your copyright notice in the Description page of Project Settings.


#include "BMAgroCheck.h"
#include "BossMode.h"
#include "BMAIController.h"
#include "BMBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DrawDebugHelpers.h"

void UBMAgroCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (ThisTree == nullptr || ThisController == nullptr || ThisAICharacter == nullptr)
	{
		ThisTree = OwnerComp.GetCurrentTree();
		ThisController = Cast<ABMAIController>(OwnerComp.GetAIOwner());
		ThisAICharacter = Cast<ABMBossCharacter>(ThisController->GetPawn());

		if (ThisTree == nullptr || ThisController == nullptr || ThisAICharacter == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Warning Agro Service performed on invalid AI"));
			return;
		}
	}
	
	// trace comples�� ������ �����ϸ鼭 sweep params ����ü�� �ʱ�ȭ �ϱ�
	FCollisionQueryParams SphereSweepParams(FName(TEXT("AgroCheckSweep")), true, ThisAICharacter);

	FCollisionObjectQueryParams ObjectQuery(ECC_GameTraceChannel1);

	// Hit Result �ʱ�ȭ �ϱ�
	FHitResult HitOut(ForceInit);

	// �ùٸ� ������ �����ϴ��� Ȯ���ϱ� ���� ����� ��ü�� �׸���.
	DrawDebugSphere(ThisAICharacter->GetWorld(), ThisAICharacter->GetActorLocation(), 1500, 12, FColor::Red, false, 4.0f);

	// ������ �����ϰ� boolean ������ üũ�Ѵ�. BMCharacter ������Ʈ�� ���ؼ��� üũ�Ѵ�.
	bool bResult = ThisAICharacter->GetWorld()->SweepSingleByObjectType(HitOut, ThisAICharacter->GetActorLocation(),
																		ThisAICharacter->GetActorLocation() + FVector(0.0f, 0.0f, 10.0f),
																		FQuat(), ObjectQuery, FCollisionShape::MakeSphere(1500), SphereSweepParams);

	if (bResult)
	{
		ThisController->GetBlackboard()->SetValueAsObject(TEXT("TargetToFollow"), HitOut.GetActor());
		ThisController->GetBlackboard()->SetValueAsVector(TEXT("HomeLocation"), ThisAICharacter->GetActorLocation());
		ThisController->GetBlackboard()->SetValueAsVector(TEXT("TargetLocation"), HitOut.GetActor()->GetActorLocation());
		ThisController->TrackToTarget();
	}
	else
	{
		ThisController->GetBlackboard()->SetValueAsObject(TEXT("TargetToFollow"), nullptr);
		ThisController->StopTrack();
	}
}
