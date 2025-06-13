#pragma once

#include "CoreMinimal.h"

#include "Board.generated.h"

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
    Dir = static_cast<EBoardDirection>(static_cast<uint8>(Dir) + 1);
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

    UPROPERTY(EditAnywhere)
    FVector Position;

    UPROPERTY()
    uint8 Flags;

    UPROPERTY()
    int32 Neighbors[static_cast<uint8>(EBoardDirection::MAX)] = {-1, -1, -1, -1, -1, -1, -1, -1};
};