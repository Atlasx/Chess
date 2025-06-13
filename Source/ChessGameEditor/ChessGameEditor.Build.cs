using UnrealBuildTool;

public class ChessGameEditor : ModuleRules
{
	public ChessGameEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "UnrealEd", "ComponentVisualizers", "ChessGame"
		});
	}
}