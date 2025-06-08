workspace "Voxels"
	filename "Voxels"
	architecture "x64"
	configurations { "Debug", "Release" }
	
	startproject "Voxel_Game"
	
	OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	ENGINE_INCLUDE_DIR = {}
	ENGINE_INCLUDE_DIR["GLM"]       = "Voxel_Game/vendors/arcane_framework/Arcane/vendors/glm"
	ENGINE_INCLUDE_DIR["SPD_LOG"]   = "Voxel_Game/vendors/arcane_framework/Arcane/vendors/spdlog/include"
	ENGINE_INCLUDE_DIR["OAL"]       = "Voxel_Game/vendors/arcane_framework/Arcane/vendors/openal-soft/include"
	ENGINE_INCLUDE_DIR["SNDFILE"]   = "Voxel_Game/vendors/arcane_framework/Arcane/vendors/libsndfile/include"
	ENGINE_INCLUDE_DIR["IMGUI"]     = "Voxel_Game/vendors/arcane_framework/Arcane/vendors/imgui"
	ENGINE_INCLUDE_DIR["ARCANE"]    = "Voxel_Game/vendors/arcane_framework/Arcane/src"
	
	group "Dependencies"
		include "Voxel_Game/vendors/arcane_framework/Arcane/vendors/glfw"
		include "Voxel_Game/vendors/arcane_framework/Arcane/vendors/glad"
		include "Voxel_Game/vendors/arcane_framework/Arcane/vendors/imgui"
		include "Voxel_Game/vendors/arcane_framework"
	
	group ""
	
project "Voxel_Game"
	location "Voxel_Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	buildoptions{"/utf-8"}
	
	targetdir("bin/" .. OUTPUT_DIR .. "/%{prj.name}")
	objdir("bin-int/" .. OUTPUT_DIR .. "/%{prj.name}")
	
	files 
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h"
	}
	
	includedirs 
	{
		"%{ENGINE_INCLUDE_DIR.ARCANE}",
		"%{ENGINE_INCLUDE_DIR.SPD_LOG}",
		"%{ENGINE_INCLUDE_DIR.IMGUI}",
		"%{ENGINE_INCLUDE_DIR.OAL}",
		"%{ENGINE_INCLUDE_DIR.SNDFILE}",
		"%{ENGINE_INCLUDE_DIR.GLM}",
		"%{prj.name}/src"
	}
	
	links {
		"Arcane"
	}
	
	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		