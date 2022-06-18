workspace "Win32Window"
	architecture "x86_64"
	configurations {
		"Debug",
		"Release"
	}

cversion = "C11"
cppversion = "C++17"

outputdir = "%{cfg.architecture}/%{cfg.system}/%{cfg.buildcfg}"
_targetdir = "bin/" .. outputdir .. "/%{prj.name}"
_objdir = _targetdir .. "/obj"

cfiles = { "%{prj.location}/inc/**.h", "%{prj.location}/src/**.c" }
cppfiles = { cfiles, "%{prj.location}/inc/**.hpp", "%{prj.location}/src/**.cpp" }

project "Win32Window"
	location "Win32Window"
	kind "StaticLib"
	language "C++"

	targetdir (_targetdir)
	objdir (_objdir)

	files {
		cppfiles
	}

	includedirs {
		"%{prj.location}/inc"
	}

	filter "system:windows"
		cppdialect (cppversion)
		staticruntime "on"
		systemversion "latest"

	filter "system:linux"
		cppdialect (cppversion)

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		defines { "NDEBUG" }

project "test"
	location "test"
	kind "ConsoleApp"
	language "C++"

	targetdir (_targetdir)
	objdir (_objdir)

	files {
		cppfiles
	}

	includedirs {
		"Win32Window/inc",
		"%{prj.location}/inc"
	}

	links {
		"Win32Window"
	}

	filter "system:windows"
		cppdialect (cppversion)
		staticruntime "on"
		systemversion "latest"

	filter "system:linux"
		cppdialect (cppversion)

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		defines { "NDEBUG" }