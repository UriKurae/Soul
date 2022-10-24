workspace "Soul"
    architecture "x64"
    startproject "Sandbox"

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
    IncludeDir["Imgui"] = "Soul/vendor/ImGui"

    group "Dependencies"
        include "Soul/vendor/GLFW"
        include "Soul/vendor/Glad"
        include "Soul/vendor/ImGui"

    group ""

    project "Soul"
        location "Soul"
        kind "SharedLib"
        language "C++"
        staticruntime "off"

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
            "%{IncludeDir.Glad}",
            "%{IncludeDir.Imgui}"
        }

        links
        {
            "GLFW",
            "Glad",
            "ImGui",
            "opengl32.lib"
        }

        filter "system:windows"
            cppdialect "C++17"
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
                runtime "Debug"
                symbols "On"

            filter "configurations:Release"
                defines "SL_RELEASE"
                runtime "Release"
                optimize "On"

            filter "configurations:Dist"
                defines "SL_DIST"
                runtime "Release"
                optimize "On"

project "Sandbox"
            location "Sandbox"
            kind "ConsoleApp"
            language "C++"
            staticruntime "off"

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
            systemversion "latest"

            defines
            {
                "SL_PLATFORM_WINDOWS"
            }

            filter "configurations:Debug"
                defines "SL_DEBUG"
                runtime "Debug"
                symbols "On"

            filter "configurations:Release"
                defines "SL_RELEASE"
                runtime "Release"
                optimize "On"

            filter "configurations:Dist"
                defines "SL_DIST"
                runtime "Release"
                optimize "On"