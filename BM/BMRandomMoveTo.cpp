// Fill out your copyright notice in the Description page of Project Settings.


#include "BMRandomMoveTo.h"
#include "BMAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AI/Navigation/NavigationTypes.h"
#include "NavigationSystem.h"

EBTNodeResult::Type
UBMRandomMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ThisTree == nullptr || ThisController == nullptr)
	{
		ThisTree = OwnerComp.GetCurrentTree();
		ThisController = Cast<ABMAIController>(OwnerComp.GetAIOwner());
		if (ThisTree == nullptr || ThisController == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Task assigned to incorrect AI type"));
			return EBTNodeResult::Failed;
		}
	}
	//auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	//UNavigationSystem* NavSystem = UNavigationSystem::GetNavigationSystem(ControllingPawn->GetWorld());
	//if (nullptr == NavSystem)
	//	return EBTNodeResult::Failed;

	//FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AZombieAIController::HomePosKey);
	//FNavLocation NextPatrol;

	//if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextPatrol))
	//{
	//	OwnerComp.GetBlackboardComponent()->SetValueAsVector(AZombieAIController::PatrolPosKey, NextPatrol.Location);
	//	return EBTNodeResult::Succeeded;
	//}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ThisController->GetWorld());
	FNavLocation RandomPoint;
	if (NavSystem->GetRandomPointInNavigableRadius(ThisController->GetPawn()->GetActorLocation(), 5000.0f, RandomPoint)) // if (GetWorld()->GetNavigationSystem()->GetRandomPointInNavigableRadius(ThisController->GetPawn()->GetActorLocation(), 5000.0f, RandomPoint))
		{
			ThisController->MoveTo(RandomPoint.Location);
			return EBTNodeResult::Succeeded;
		}
	return EBTNodeResult::Failed;
}