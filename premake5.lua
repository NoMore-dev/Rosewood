workspace "Rosewood"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Rosewood/vendor/GLFW/include"
IncludeDir["glad"] = "Rosewood/vendor/glad/include"
IncludeDir["imgui"] = "Rosewood/vendor/imgui"
IncludeDir["glm"] = "Rosewood/vendor/glm"

include "Rosewood/vendor/GLFW"
include "Rosewood/vendor/glad"
include "Rosewood/vendor/imgui"

project "Rosewood"
	location "Rosewood"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rwpch.h"
	pchsource "Rosewood/src/rwpch.cpp"

	defines
	{			
		"_CRT_SECURE_NO_WARNINGS"
	}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}"
	}

	links 
	{ 
		"GLFW",
		"glad",
		"imgui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{			
			"RW_PLATFORM_WINDOWS",
			"RW_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "RW_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RW_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RW_DIST"
		runtime "Release"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Rosewood/vendor/spdlog/include",
		"Rosewood/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Rosewood"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{			
			"RW_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RW_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RW_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RW_DIST"
		runtime "Release"
		optimize "on"
