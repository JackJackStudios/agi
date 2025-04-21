project "agi"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    
	pchheader "agipch.h"
	pchsource "src/agipch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "include/**.h",

        "lib/glad/src/**.c",
    }

    includedirs
    {
        "src",
        "include",
        "lib/glad/include",
        "lib/stb_image",
        "lib/glm"
    }

    links
    {
    }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

    filter "files:lib/glad/src/glad.c"
        flags { "NoPCH" }
    
    filter "configurations:Debug"
        defines "AGI_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "AGI_RELEASE"
        runtime "Release"
        symbols "on"