// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class StrandedInAnIslandTarget : TargetRules
{
	public StrandedInAnIslandTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "StrandedInAnIsland" } );
	}
}
