

project "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp",
		"imgui_impl_opengl3.cpp",
		"imgui_impl_opengl3.h",
		"imgui_impl_glfw.cpp",
		"imgui_impl_glfw.h"
	}

	sysincludedirs
	{
		"../glfw/include/",
		"../glew-2.1.0/include/"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Production"
		runtime "Release"
		optimize "on"


	