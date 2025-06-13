#include "ChessGameEditor.h"
#include "Board/BoardComponentVisualizer.h"
#include "UnrealEd.h"
#include "Board/BoardComponent.h"

IMPLEMENT_MODULE(FChessGameEditorModule, ChessGameEditor)

void FChessGameEditorModule::StartupModule()
{
	if (GUnrealEd)
	{
		TSharedPtr<FComponentVisualizer> Visualizer = MakeShareable(new FBoardComponentVisualizer());
		GUnrealEd->RegisterComponentVisualizer(UBoardComponent::StaticClass()->GetFName(), Visualizer);
		Visualizer->OnRegister();
	}
}

void FChessGameEditorModule::ShutdownModule()
{
	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(UBoardComponent::StaticClass()->GetFName());
	}
}

