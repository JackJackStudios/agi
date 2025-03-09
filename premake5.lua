project "agi"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

    includedirs
    {
        "src",
        "%{wks.location}/lib/glm/",
        "%{wks.location}/lib/glm/",
        "%{wks.location}/lib/glad/include/",
    }

    links
    {
        "glad",
    }

    filter "configurations:Debug"
        defines "AGI_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "AGI_RELEASE"
        runtime "Release"
        symbols "on"

group "Dependencies"
    include "lib/glad/"
group ""