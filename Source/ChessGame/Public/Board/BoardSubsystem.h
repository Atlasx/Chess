// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "BoardSubsystem.generated.h"

enum class EBoardDirection : uint8
{
    North,
    East,
    South,
    West,
    Northwest,
    Northeast,
    Southwest,
    Southeast,
    MAX
};

enum class ENodeFlags : uint8
{
    None = 0 UMETA(Hidden),
    Navigable = 1 << 0
};
ENUM_CLASS_FLAGS(ENodeFlags)

USTRUCT()
struct FBoardNode
{
    GENERATED_BODY()

    UPROPERTY()
    FVector Position;

    UPROPERTY()
    uint8 Flags;

    UPROPERTY()
    int32 Neighbors[static_cast<uint8>(EBoardDirection::MAX)];
};

UCLASS()
class CHESSGAME_API ABoardActor : public AActor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TArray<FBoardNode> GraphNodes;

    TArray<FBoardNode*> ExternalConnections;

public:
    /* Connection Management */
    UFUNCTION(CallInEditor)
    void BuildGraph();

    UFUNCTION(BlueprintCallable, CallInEditor)
    void RebuildGraph();

    void ConnectNodes(const FBoardNode& First, const FBoardNode& Second, EBoardDirection Dir);
    void DisconnectNodes(const FBoardNode& First, const FBoardNode& Second);

    UFUNCTION(BlueprintCallable)
    void ConnectExternal(ABoardActor* Other);

    UFUNCTION(BlueprintCallable)
    void DisconnectExternal(ABoardActor* Other);

    /* Navigation Requests */
    FBoardNode* GetNodeAtLocation(const FVector& Location, float Tolerance = 50.f) const;
    FBoardNode* GetClosestNode(const FVector& Location) const;

    bool HasPath(const FBoardNode* From, const FBoardNode* To) const;
    bool FindPath(const FBoardNode* Start, const FBoardNode* End, TArray<const FBoardNode*>& OutPath) const;

    /* Debug */
    UFUNCTION(CallInEditor)
    void DrawDebugGraph() const;

    UFUNCTION(CallInEditor)
    void DrawDebugPath(const TArray<FVector>& Path, FColor Color = FColor::Green) const;

protected:
    inline bool GetNodeNeighbor(const FBoardNode& node,const EBoardDirection& direction, FBoardNode* OutNode);
    inline bool GetConstNodeNeighbor(const FBoardNode& node, const EBoardDirection& direction, const FBoardNode* OutNode) const;
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
