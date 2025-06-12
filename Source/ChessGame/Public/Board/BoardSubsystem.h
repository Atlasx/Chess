// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "BoardSubsystem.generated.h"

enum class EBoardDirection : uint8
{
    Up,
    Right,
    Down,
    Left,
    UpRight,
    DownRight,
    UpLeft,
    DownLeft,
    MAX
};

inline EBoardDirection& operator++(EBoardDirection& Dir)
{
    Dir = static_cast<EBoardDirection>((static_cast<uint8>(Dir) + 1) % static_cast<uint8>(EBoardDirection::MAX));
    return Dir;
}

static EBoardDirection ReverseDirection(const EBoardDirection& Dir)
{
    switch (Dir)
    {
    case EBoardDirection::Up: return EBoardDirection::Down;
    case EBoardDirection::Right: return EBoardDirection::Left;
    case EBoardDirection::Down: return EBoardDirection::Up;
    case EBoardDirection::Left: return EBoardDirection::Right;
    case EBoardDirection::UpRight: return EBoardDirection::DownLeft;
    case EBoardDirection::DownRight: return EBoardDirection::UpLeft;
    case EBoardDirection::UpLeft: return EBoardDirection::DownRight;
    case EBoardDirection::DownLeft: return EBoardDirection::UpRight;
    }
    return EBoardDirection::MAX;
}

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
class CHESSGAME_API UBoardComponent : public USceneComponent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TArray<FBoardNode> Nodes;
    TArray<FBoardNode*> ExternalNodes;

public:
    virtual FVector DirectionToOffset(const EBoardDirection& Dir);
    //virtual EBoardDirection OffsetToDirection(const FVector& Offset);

    UFUNCTION(CallInEditor)
    void BuildGraph();

    void AddNode(FVector Location, uint8 Flags);
    void AddNode(FBoardNode* Neighbor, const EBoardDirection& Dir, uint8 Flags);

    void RemoveNode(FVector Location);

    int32 GetNodeIndex(FBoardNode* Node);

    /* Find the node within a tolerance of a location */
    bool FindNodeAt(FVector Location, FBoardNode* OutNode, float Tolerance = 10.f);

    /* Find the node closest to the location */
    bool FindClosestNode(FVector Location, FBoardNode* OutNode);

    void RebuildGraph();
};

UCLASS()
class CHESSGAME_API ABoardActor : public AActor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBoardComponent* BoardComponent;

public:
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
