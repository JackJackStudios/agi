project "agi"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",
        "lib/glad/include",
        "lib/stb_image",
        "lib/glm/"
    }

    links
    {
        "glad",
    }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}
    
    filter "configurations:Debug"
        defines "AGI_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "AGI_RELEASE"
        runtime "Release"
        symbols "on"
    