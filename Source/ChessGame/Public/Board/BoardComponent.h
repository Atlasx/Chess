#pragma once

#include "CoreMinimal.h"

#include "Board.h"

#include "BoardComponent.generated.h"

class FPrimitiveDrawInterface;

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

    void DrawDebugGraph(FPrimitiveDrawInterface* PDI) const;
};
