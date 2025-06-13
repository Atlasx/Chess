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

ABoardActor::ABoardActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BoardComponent = CreateDefaultSubobject<UBoardComponent>(TEXT("BoardComponent"));
	SetRootComponent(BoardComponent);
}

void ABoardActor::ConnectExternal(ABoardActor* Other)
{

}

void ABoardActor::DisconnectExternal(ABoardActor* Other)
{

}


void ABoardActor::DrawDebugGraph() const
{

}

void ABoardActor::DrawDebugPath(const TArray<FVector>& Path, FColor Color /*= FColor::Green*/) const
{
	
}

