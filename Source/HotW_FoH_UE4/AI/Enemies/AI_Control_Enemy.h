// © 2026 Ricardo Sánchez Villegas. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_Control_Enemy.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;	

UCLASS()
class HOTW_FOH_UE4_API AAI_Control_Enemy : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAI_Control_Enemy();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	// Component that handles the execution flow of the Behavior Tree
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTreeComponent* BehaviorTreeComp;

	// Component representing the AI's custom "memory bank"
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	// The asset pointer to the Behavior Tree created in Editor
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Convenient getter for the Blackboard component
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComp; }
};
