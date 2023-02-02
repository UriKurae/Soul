workspace "Soul"
    architecture "x64"
    startproject "SoulEditor"

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
    IncludeDir["glm"] = "Soul/vendor/glm"
    IncludeDir["stb_image"] = "Soul/vendor/stb_image"
    IncludeDir["entt"] = "Soul/vendor/entt/include"
    IncludeDir["yaml_cpp"] = "Soul/vendor/yaml-cpp/include"
    IncludeDir["Assimp"] = "Soul/vendor/Assimp/include"

    group "Dependencies"
        include "Soul/vendor/GLFW"
        include "Soul/vendor/Glad"
        include "Soul/vendor/ImGui"
        include "Soul/vendor/yaml-cpp"
        include "Soul/vendor/Assimp"

    group ""

    project "Soul"
        location "Soul"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "slpch.h"
        pchsource "Soul/src/slpch.cpp"

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/vendor/stb_image/**.h",
            "%{prj.name}/vendor/stb_image/**.cpp",
            "%{prj.name}/vendor/glm/glm/**.hpp",
            "%{prj.name}/vendor/glm/glm/**.inl"
        }

        defines 
        {
            "_CRT_SECURE_NO_WARNINGS"
        }

        includedirs
        {
            "%{prj.name}/src",
            "%{prj.name}/vendor/spdlog/include",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.Imgui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb_image}",
            "%{IncludeDir.entt}",
            "%{IncludeDir.yaml_cpp}",
            "%{IncludeDir.Assimp}"
        }

        links
        {
            "GLFW",
            "Glad",
            "ImGui",
            "yaml-cpp",
            "opengl32.lib",
            "Assimp"
        }

        filter "system:windows"
            systemversion "latest"

            defines
            {
                "SL_PLATFORM_WINDOWS",
                "SL_BUILD_DLL",
                "GLFW_INCLUDE_NONE"
            }

            filter "configurations:Debug"
                defines "SL_DEBUG"
                runtime "Debug"
                symbols "on"

            filter "configurations:Release"
                defines "SL_RELEASE"
                runtime "Release"
                optimize "on"

            filter "configurations:Dist"
                defines "SL_DIST"
                runtime "Release"
                optimize "on"

project "Sandbox"
            location "Sandbox"
            kind "ConsoleApp"
            language "C++"
            cppdialect "C++17"
            staticruntime "on"

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
            "Soul/src",
            "Soul/vendor",
            "%{IncludeDir.glm}",
            "%{IncludeDir.entt}"
        }

        links
        {
            "Soul"
        }

        filter "system:windows"
            systemversion "latest"

            defines
            {
                "SL_PLATFORM_WINDOWS"
            }

            filter "configurations:Debug"
                defines "SL_DEBUG"
                runtime "Debug"
                symbols "on"

            filter "configurations:Release"
                defines "SL_RELEASE"
                runtime "Release"
                optimize "on"

            filter "configurations:Dist"
                defines "SL_DIST"
                runtime "Release"
                optimize "on"

project "SoulEditor"
            location "SoulEditor"
            kind "ConsoleApp"
            language "C++"
            cppdialect "C++17"
            staticruntime "on"

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
            "Soul/src",
            "Soul/vendor",
            "%{IncludeDir.glm}",
            "%{IncludeDir.entt}"
        }

        links
        {
            "Soul"
        }

        filter "system:windows"
            systemversion "latest"

            defines
            {
                "SL_PLATFORM_WINDOWS"
            }

            filter "configurations:Debug"
                defines "SL_DEBUG"
                runtime "Debug"
                symbols "on"

            filter "configurations:Release"
                defines "SL_RELEASE"
                runtime "Release"
                optimize "on"

            filter "configurations:Dist"
                defines "SL_DIST"
                runtime "Release"
                optimize "on"