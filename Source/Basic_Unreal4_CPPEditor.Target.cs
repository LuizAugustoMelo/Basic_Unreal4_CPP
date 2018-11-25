// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Basic_Unreal4_CPPEditorTarget : TargetRules
{
	public Basic_Unreal4_CPPEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Basic_Unreal4_CPP");
	}
}
