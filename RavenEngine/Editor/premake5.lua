

project "RavenEditor"
	kind "ConsoleApp"
	language "C++"
	editandcontinue "Off"
	debugdir (root_dir.."/gameProject/")
	
	files
	{
		"**.h",
		"**.cpp",
		"Source/**.h",
		"Source/**.cpp"
	}
	
	includedirs
	{
		"../RavenEngine/Source",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glew}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.cereal}",
		"%{IncludeDir.Raven}",
		"%{IncludeDir.OpenFBX}",
		"%{IncludeDir.glm}",
	}

	sysincludedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glew}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Dependencies}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.cereal}",
		"%{IncludeDir.Raven}",
		"%{IncludeDir.OpenFBX}",
		"%{IncludeDir.glm}",
	}

	links
	{
		"RavenEngine",
		"imgui",
		"spdlog",
	}

	defines
	{
		"SPDLOG_COMPILED_LIB"
	}

	filter { "files:Dependencies/**"}
		warnings "Off"

	filter 'architecture:x86_64'
		defines { "RAVEN_SSE"}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		--entrypoint "WinMainCRTStartup"
		--entrypoint "mainCRTStartup"

		defines
		{

			"_CRT_SECURE_NO_WARNINGS",
			"_DISABLE_EXTENDED_ALIGNED_STORAGE",
			"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING",
		}

		libdirs
		{
			--"../RAVEN/Dependencies/libs" 
		}

		links
		{
			"glfw",
			"OpenGL32",
		}

		disablewarnings { 4307 }

	filter "configurations:Debug"
		defines { "LUMOS_DEBUG", "_DEBUG","TRACY_ENABLE","LUMOS_PROFILE", }
		symbols "On"
		runtime "Debug"
		optimize "Off"

	filter "configurations:Release"
		defines { "LUMOS_RELEASE","TRACY_ENABLE", "LUMOS_PROFILE",}
		optimize "Speed"
		symbols "On"
		runtime "Release"

	filter "configurations:Production"
		defines "LUMOS_PRODUCTION"
		symbols "Off"
		optimize "Full"
		runtime "Release"
