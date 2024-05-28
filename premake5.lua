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

include "Rosewood/vendor/GLFW"
include "Rosewood/vendor/glad"
include "Rosewood/vendor/imgui"

project "Rosewood"
	location "Rosewood"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rwpch.h"
	pchsource "Rosewood/src/rwpch.cpp"

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
		"%{IncludeDir.imgui}"
	}

	links 
	{ 
		"GLFW",
		"glad",
		"imgui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{			
			"RW_PLATFORM_WINDOWS",
			"RW_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "RW_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RW_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "RW_DIST"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		"Rosewood/src"
	}

	links
	{
		"Rosewood"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{			
			"RW_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RW_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RW_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "RW_DIST"
		runtime "Release"
		optimize "On"