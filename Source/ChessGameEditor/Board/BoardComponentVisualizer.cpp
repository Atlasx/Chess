#include "BoardComponentVisualizer.h"

#include "Board/BoardComponent.h"

void FBoardComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UBoardComponent* BoardComponent = Cast<const UBoardComponent>(Component);
	if (!BoardComponent) { return; }

	BoardComponent->DrawDebugGraph(PDI);
}
