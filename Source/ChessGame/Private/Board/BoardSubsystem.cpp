// Fill out your copyright notice in the Description page of Project Settings.


#include "Board/BoardSubsystem.h"

/*
* Board Subsystem
*/
void UBoardSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UWorld* World = GetWorld();
	if (World)
	{
		ActorSpawnedHandle = World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &UBoardSubsystem::OnActorSpawned));
	}
}

void UBoardSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UBoardSubsystem::RegisterBoard(ABoardActor* Board)
{

}

void UBoardSubsystem::UnregisterBoard(ABoardActor* Board)
{

}

void UBoardSubsystem::ConnectBoards(ABoardActor* First, ABoardActor* Second)
{

}

void UBoardSubsystem::DisconnectBoards(ABoardActor* First, ABoardActor* Second)
{

}

void UBoardSubsystem::OnActorSpawned(AActor* SpawnedActor)
{
	if (ABoardActor* Board = Cast<ABoardActor>(SpawnedActor))
	{
		RegisterBoard(Board);
	}
}

/*
* Board Actor
*/

void ABoardActor::BuildGraph()
{

}



void ABoardActor::RebuildGraph()
{

}

void ABoardActor::ConnectNodes(const FBoardNode& First, const FBoardNode& Second, EBoardDirection Dir)
{

}

void ABoardActor::DisconnectNodes(const FBoardNode& First, const FBoardNode& Second)
{

}

void ABoardActor::ConnectExternal(ABoardActor* Other)
{

}

void ABoardActor::DisconnectExternal(ABoardActor* Other)
{

}

FBoardNode* ABoardActor::GetNodeAtLocation(const FVector& Location, float Tolerance /*= 50.f*/) const
{
	return nullptr;
}

FBoardNode* ABoardActor::GetClosestNode(const FVector& Location) const
{
	return nullptr;
}

bool ABoardActor::HasPath(const FBoardNode* From, const FBoardNode* To) const
{
	return false;
}

bool ABoardActor::FindPath(const FBoardNode* Start, const FBoardNode* End, TArray<const FBoardNode*>& OutPath) const
{
	return false;
}

void ABoardActor::DrawDebugGraph() const
{
	const float NodeDrawSize = 20.f;
	const UWorld* World = GetWorld();

	for (const FBoardNode& node : GraphNodes)
	{
		// Draw Node
		FColor NodeColor = FColor::Red;
		if (node.Flags & static_cast<uint8>(ENodeFlags::Navigable)) { NodeColor = FColor::Blue; }
		DrawDebugSphere(World, node.Position, NodeDrawSize, 12, NodeColor);

		// Draw Connections
		for (uint8 dir = 0; dir < static_cast<uint8>(EBoardDirection::MAX); ++dir)
		{
			FBoardNode* other = nullptr;
			EBoardDirection nodeDir = static_cast<EBoardDirection>(dir);
			if (GetConstNodeNeighbor(node, nodeDir, other) == false) { continue; }
			
			DrawDebugLine(World, node.Position, other->Position, FColor::Blue);
		}
	}
}

void ABoardActor::DrawDebugPath(const TArray<FVector>& Path, FColor Color /*= FColor::Green*/) const
{
	
}

inline bool ABoardActor::GetNodeNeighbor(const FBoardNode& node, const EBoardDirection& direction, const FBoardNode& OutNode)
{
	int32 otherIndex = node.Neighbors[static_cast<uint8>(direction)];
	if (otherIndex >= GraphNodes.Num())
	{ 
		OutNode = nullptr;
		return false;
	}

	OutNode = &GraphNodes[otherIndex];
	return OutNode != nullptr;
}

bool ABoardActor::GetConstNodeNeighbor(const FBoardNode& node, const EBoardDirection& direction, const FBoardNode* OutNode) const
{
	FBoardNode* OtherNode;
	if (GetNodeNeighbor(node, direction, OtherNode))
	{
		OutNode = const_cast<const FBoardNode*>(OtherNode);
		return true;
	}
	return false;
}
