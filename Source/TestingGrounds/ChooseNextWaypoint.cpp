// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get Patrol Points
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Guard %s missing Patrol Points"),*ControlledPawn->GetName())
		return EBTNodeResult::Failed;
	}

	// Set next waypoint
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComponent->SetValueAsObject(NextWaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// Cycle Index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}