// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ComponentVisualizer.h"

#include "Board/Board.h"
#include "Board/BoardComponent.h"

#include "BoardSubsystem.generated.h"

UCLASS()
class CHESSGAME_API ABoardActor : public AActor
{
    GENERATED_BODY()

public:
    ABoardActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
    UBoardComponent* BoardComponent;

    /* Connection Management */

    UFUNCTION(BlueprintCallable)
    void ConnectExternal(ABoardActor* Other);

    UFUNCTION(BlueprintCallable)
    void DisconnectExternal(ABoardActor* Other);

    /* Debug */
    UFUNCTION(CallInEditor)
    void DrawDebugGraph() const;

    UFUNCTION(CallInEditor)
    void DrawDebugPath(const TArray<FVector>& Path, FColor Color = FColor::Green) const;
};

/**
 * 
 */
UCLASS()
class CHESSGAME_API UBoardSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
    UPROPERTY()
    TArray<ABoardActor*> Boards;

    FDelegateHandle ActorSpawnedHandle;

public:
    /* UWorldSubsystem */
    void Initialize(FSubsystemCollectionBase& Collection) override;
    void Deinitialize() override;

    /* Adding New Boards */
    void RegisterBoard(ABoardActor* Board);
    void UnregisterBoard(ABoardActor* Board);
    void OnActorSpawned(AActor* SpawnedActor);

    /* Connecting Boards Together */
    void ConnectBoards(ABoardActor* First, ABoardActor* Second);
    void DisconnectBoards(ABoardActor* First, ABoardActor* Second);

    // Handle any navigation requests from actors with a criteria to fulfill
    //void GetNavigableNodes(FVector Start, TArray<FBoardNode*>& Nodes, CriteriaFunc);

};
