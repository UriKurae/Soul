workspace "Soul"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    -- Include directiories relative to root foler (In this case, solution directory)
    IncludeDir = {}
    IncludeDir["GLFW"] = "Soul/vendor/GLFW/include"
    IncludeDir["Glad"] = "Soul/vendor/Glad/include"

    include "Soul/vendor/GLFW"
    include "Soul/vendor/Glad"

    project "Soul"
        location "Soul"
        kind "SharedLib"
        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "slpch.h"
        pchsource "Soul/src/slpch.cpp"

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        }

        includedirs
        {
            "%{prj.name}/src",
            "%{prj.name}/vendor/spdlog/include",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}"
        }

        links
        {
            "GLFW",
            "Glad",
            "opengl32.lib"
        }

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"

            defines
            {
                "SL_PLATFORM_WINDOWS",
                "SL_BUILD_DLL",
                "GLFW_INCLUDE_NONE"
            }

            postbuildcommands
            {
                ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
            }

            filter "configurations:Debug"
                defines "SL_DEBUG"
                buildoptions "/MDd"
                symbols "On"

            filter "configurations:Release"
                defines "SL_RELEASE"
                 buildoptions "/MD"
                optimize "On"

            filter "configurations:Dist"
                defines "SL_DIST"
                 buildoptions "/MD"
                optimize "On"

project "Sandbox"
            location "Sandbox"
            kind "ConsoleApp"
            language "C++"

            targetdir ("bin/" .. outputdir .. "/%{prj.name}")
            objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        }

        includedirs 
        {
            "Soul/vendor/spdlog/include",
            "Soul/src"
        }

        links
        {
            "Soul"
        }

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"

            defines
            {
                "SL_PLATFORM_WINDOWS"
            }

            filter "configurations:Debug"
                defines "SL_DEBUG"
                buildoptions "/MDd"
                symbols "On"

            filter "configurations:Release"
                defines "SL_RELEASE"
                buildoptions "/MD"
                optimize "On"

            filter "configurations:Dist"
                defines "SL_DIST"
                buildoptions "/MD"
                optimize "On"