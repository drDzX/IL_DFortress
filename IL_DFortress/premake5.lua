workspace "IL_DFortress"
	architecture "x86"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "IL_DFortress"
	location "IL_DFortress"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("Intermediate/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
	"%{prj.name}/3rdParty/boost_1_74_0",
	"%{prj.name}/3rdParty/pugixml-master/src",
	"%{prj.name}/3rdParty/json-develop/single_include/nlohmann",
	"%{prj.name}/Source",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		

	postbuildcommands
	{
	  "{mkdir} %{wks.location}/bin/"..outputdir.."/%{prj.name}/Config",
	  "{copy} Config %{wks.location}/bin/"..outputdir.."/%{prj.name}/Config",
	}

	filter "configurations:Debug"
		defines "DZX_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "DZX_RELEASE"
		optimize "On"
			