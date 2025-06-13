#include "Board/BoardComponent.h"

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

void UBoardComponent::BuildGraph()
{
	FVector location(0.f, 0.f, 0.f);
	FVector offset(100.f, 0.f, 0.f);

	for (int i = 0; i < 10; ++i)
	{
		AddNode(location + i * offset, 0);
	}
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

void UBoardComponent::DrawDebugGraph(FPrimitiveDrawInterface* PDI) const
{
	constexpr float NodeDrawSize = 20.f;

	for (const FBoardNode& node : Nodes)
	{
		// Draw Node
		FColor NodeColor = FColor::Red;
		if (node.Flags & static_cast<uint8>(ENodeFlags::Navigable)) { NodeColor = FColor::Blue; }
		PDI->DrawPoint(node.Position, NodeColor, NodeDrawSize, SDPG_World);

		// Draw Connections
		for (uint8 dir = 0; dir < static_cast<uint8>(EBoardDirection::MAX); ++dir)
		{
			const FBoardNode* other = nullptr;
			EBoardDirection nodeDir = static_cast<EBoardDirection>(dir);

			int32 neighborIndex = node.Neighbors[static_cast<int32>(nodeDir)];
			if (neighborIndex != -1)
			{
				other = &Nodes[neighborIndex];
			}

			if (other)
			{
				PDI->DrawLine(node.Position, other->Position, FColor::Blue, SDPG_World);
			}
		}
	}
}

