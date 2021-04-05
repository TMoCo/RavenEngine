

project "rp3d"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    files {
        "src/**.c",
        "src/**.cpp"
    }

    includedirs {
        "include"
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


