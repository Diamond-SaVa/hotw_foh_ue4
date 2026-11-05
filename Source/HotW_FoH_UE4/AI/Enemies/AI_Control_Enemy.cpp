// © 2026 Ricardo Sánchez Villegas. All rights reserved. 


#include "AI_Control_Enemy.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
AAI_Control_Enemy::AAI_Control_Enemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AAI_Control_Enemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BehaviorTreeAsset)
	{
		// Initialize the blackboard data layout using the behavior tree asset's default blackboard
		if (BehaviorTreeAsset->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(BehaviorTreeAsset->BlackboardAsset));
		}

		// Run the custom behavior tree
		BehaviorTreeComp->StartTree(*BehaviorTreeAsset);
	}
}

// Called when the game starts or when spawned
void AAI_Control_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAI_Control_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

