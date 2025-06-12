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
* Board Component
*/
FVector UBoardComponent::DirectionToOffset(const EBoardDirection& Dir)
{
	switch (Dir)
	{
	case EBoardDirection::Up: return FVector(0.f, 0.f, 1.f);
	case EBoardDirection::Right: return FVector(1.f, 0.f, 0.f);
	case EBoardDirection::Down: return FVector(0.f, 0.f, -1.f);
	case EBoardDirection::Left: return FVector(-1.f, 0.f, 0.f);
	case EBoardDirection::UpRight: return FVector(1.f, 0.f, 1.f);
	case EBoardDirection::DownRight: return FVector(1.f, 0.f, -1.f);
	case EBoardDirection::UpLeft: return FVector(-1.f, 0.f, 1.f);
	case EBoardDirection::DownLeft: return FVector(-1.f, 0.f, -1.f);
	}
	return FVector::ZeroVector;
}

void UBoardComponent::AddNode(FVector Location, uint8 Flags)
{
	const FVector localPosition = GetComponentTransform().InverseTransformPosition(Location);
	const int32 nodeIndex = Nodes.Emplace(localPosition, Flags);
	FBoardNode* node = &Nodes[nodeIndex];

	// Find neighboring nodes and setup logical connections
	for (EBoardDirection Dir = EBoardDirection::Up; Dir != EBoardDirection::MAX; ++Dir)
	{
		FVector searchLocation = localPosition + DirectionToOffset(Dir);
		FBoardNode* neighbor = nullptr;
		if (FindNodeAt(searchLocation, neighbor))
		{
			const int32 neighborIndex = GetNodeIndex(neighbor);
			node->Neighbors[static_cast<int32>(Dir)] = neighborIndex;
			neighbor->Neighbors[static_cast<int32>(ReverseDirection(Dir))] = nodeIndex;
		}
	}
}

void UBoardComponent::AddNode(FBoardNode* Neighbor, const EBoardDirection& Dir, uint8 Flags)
{
	UE_LOG(LogTemp, Error, TEXT("Used wrong AddNode"));
}

int32 UBoardComponent::GetNodeIndex(FBoardNode* Node)
{
	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (&Nodes[i] == Node)
		{
			return i;
		}
	}

	return -1;
}

// Could use a A* search here to get closer to the location
bool UBoardComponent::FindNodeAt(FVector Location, FBoardNode* OutNode, float Tolerance /*= 10.f*/)
{
	const float sqrTolerance = Tolerance * Tolerance;
	const FVector localPosition = GetComponentTransform().InverseTransformPosition(Location);

	for (FBoardNode& node : Nodes)
	{
		if (FVector::DistSquared(localPosition, node.Position) <= sqrTolerance)
		{
			OutNode = &node;
			return true;
		}
	}

	return false;
}

bool UBoardComponent::FindClosestNode(FVector Location, FBoardNode* OutNode)
{
	float SqrNearest = MAX_FLT;
	FBoardNode* NearestNode = nullptr;

	for (FBoardNode& node : Nodes)
	{
		const float SqrDist = FVector::DistSquared(Location, node.Position);
		if (SqrDist < SqrNearest)
		{
			SqrNearest = SqrDist;
			NearestNode = &node;
		}
	}

	if (NearestNode == nullptr) { return false; }

	OutNode = NearestNode;
	return true;
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
			//if (GetConstNodeNeighbor(node, nodeDir, other) == false) { continue; }
			
			DrawDebugLine(World, node.Position, other->Position, FColor::Blue);
		}
	}
}

void ABoardActor::DrawDebugPath(const TArray<FVector>& Path, FColor Color /*= FColor::Green*/) const
{
	
}
