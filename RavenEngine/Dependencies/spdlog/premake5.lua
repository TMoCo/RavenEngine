project "spdlog"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	disablewarnings {
		"4251",
		"4275",
	}

	defines {
		"SPDLOG_COMPILED_LIB",
	}

	files {
		"src/**.cpp",
		"include/**.h",
	}

	sysincludedirs 
	{
		"include",
	}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "NDEBUG"
		optimize "On"

	filter "configurations:Production"
		optimize "On"
