// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;

using UnrealBuildTool;

public class OpenCVTest2 : ModuleRules
{
	public OpenCVTest2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "RHI", "RenderCore"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		// LoadOpenCV(Target);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
	
	public bool LoadOpenCV(ReadOnlyTargetRules Target)
	{
		// Start OpenCV linking here!
		bool isLibrarySupported = false;

		// Create OpenCV Path 
		string OpenCVPath = Path.Combine(ModuleDirectory, "ThirdParty", "OpenCV");

		// Get Library Path 
		string LibPath = "";
		// bool isdebug = Target.Configuration == UnrealTargetConfiguration.Debug && BuildConfiguration.bDebugBuildsActuallyUseDebugCRT;
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			LibPath = Path.Combine(OpenCVPath, "lib", "x64");
			isLibrarySupported = true;
		}
		else
		{
			string Err =
				string.Format(
					"{0} dedicated server is made to depend on {1}. We want to avoid this, please correct module dependencies.",
					Target.Platform.ToString(), this.ToString());
			System.Console.WriteLine(Err);
		}

		if (isLibrarySupported)
		{
			//Add Include path 
			PublicIncludePaths.AddRange(new string[] {Path.Combine(OpenCVPath, "include")});

			// Add Library Path 
			// PublicLibraryPaths.Add(LibPath);
			PublicSystemIncludePaths.Add(Path.Combine(OpenCVPath, "include"));

			//Add Static Libraries
			// PublicAdditionalLibraries.Add(Path.Combine(LibPath, "opencv_core451.lib"));
			// PublicAdditionalLibraries.Add(Path.Combine(LibPath, "opencv_videoio451.lib"));
			// PublicAdditionalLibraries.Add(Path.Combine(LibPath, "opencv_highgui451.lib"));
			// PublicAdditionalLibraries.Add(Path.Combine(LibPath, "opencv_imgproc451.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, "opencv_world451.lib"));

			//Add Dynamic Libraries
			// PublicDelayLoadDLLs.Add("opencv_core451.dll");
			// PublicDelayLoadDLLs.Add("opencv_videoio451.dll");
			// PublicDelayLoadDLLs.Add("opencv_highgui451.dll");
			// PublicDelayLoadDLLs.Add("opencv_imgproc451.dll");
			// PublicDelayLoadDLLs.Add(Path.Combine("opencv_ffmpeg320_64.dll"));

			// Add DLL to Runtime Dependencies
			// RuntimeDependencies.Add(Path.Combine(LibPath, "opencv_core451.dll"));
			// RuntimeDependencies.Add(Path.Combine(LibPath, "opencv_videoio451.dll"));
			// RuntimeDependencies.Add(Path.Combine(LibPath, "opencv_highgui451.dll"));
			// RuntimeDependencies.Add(Path.Combine(LibPath, "opencv_imgproc451.dll"));
			
			// AddDllWin64(LibPath, "opencv_core451.dll");
			// AddDllWin64(LibPath, "opencv_videoio451.dll");
			// AddDllWin64(LibPath, "opencv_highgui451.dll");
			// AddDllWin64(LibPath, "opencv_imgproc451.dll");
			
			PublicLibraryPaths.Add(LibPath);
			AddDllWin64(LibPath, "opencv_world451.dll");
			AddDllWin64(LibPath, "opencv_videoio_ffmpeg451_64.dll");

		}

		PublicDefinitions.Add(string.Format("WITH_OPENCV_BINDING={0}", isLibrarySupported ? 1 : 0));

		return isLibrarySupported;
	}
	
	private void AddDllWin64(string dllLocatedDir, string dllName)
	{
		var fullPath = Path.Combine(dllLocatedDir, dllName);

		PublicAdditionalLibraries.Add(fullPath);
		PublicDelayLoadDLLs.Add(dllName);
		RuntimeDependencies.Add(fullPath);
		CopyDll(dllName, fullPath);
	}

	private void CopyDll(string dllName, string dllFullPath)
	{
		if (!File.Exists(dllFullPath))
		{
			Console.WriteLine("file {0} does not exist", dllName);
			return;
		}

		var binariesDir = Path.Combine(ModuleDirectory, "../../Binaries/Win64/");

		if (!Directory.Exists(binariesDir))
		{
			Directory.CreateDirectory(binariesDir);
		}

		var binariesDllFullPath = Path.Combine(binariesDir, dllName);

		if (File.Exists(binariesDllFullPath))
		{
			File.SetAttributes(binariesDllFullPath, File.GetAttributes(binariesDllFullPath) & ~FileAttributes.ReadOnly);
		}

		try
		{
			File.Copy(dllFullPath, binariesDllFullPath, true);
		}
		catch (Exception e)
		{
			Console.WriteLine("failed to copy dll file: {0}", e);
		}
	}
}
