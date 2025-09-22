VULKAN_SDK = os.getenv("VULKAN_SDK")

workspace "Rosewood"
	architecture "x64"
	startproject "ModelViewer"

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
IncludeDir["stb"] = "Rosewood/vendor/stb_image"
IncludeDir["entt"] = "Rosewood/vendor/entt/include"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

include "Rosewood/vendor/GLFW"
include "Rosewood/vendor/glad"
include "Rosewood/vendor/imgui"


project "Rosewood"
	location "Rosewood"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.VulkanSDK}",
	}

	links 
	{ 
		"GLFW",
		"glad",
		"imgui",

		"%{VULKAN_SDK}/Lib/spirv-cross-core.lib",
		"%{VULKAN_SDK}/Lib/spirv-cross-cpp.lib",
		"%{VULKAN_SDK}/Lib/spirv-cross-glsl.lib",
		"%{VULKAN_SDK}/Lib/spirv-cross-reflect.lib",
		"%{VULKAN_SDK}/Lib/spirv-cross-util.lib",
		"%{VULKAN_SDK}/Lib/shaderc_combined.lib",

		"opengl32.lib",
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
		

project "ModelViewer"
	location "Examples/ModelViewer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Examples/%{prj.name}/src/**.h",
		"Examples/%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Examples/%{prj.name}/src",
		"Rosewood/vendor/spdlog/include",
		"Rosewood/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.entt}"
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