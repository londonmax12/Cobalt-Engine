outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Ikigai"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

    group "Core"
        project "Ikigai-Engine"
            location "projects/Ikigai-Engine"
            kind "StaticLib"
            language "C++"
            cppdialect "C++20"
            staticruntime "off"

            targetdir ("bin/" .. outputdir .. "/%{prj.name}")
            objdir ("bin/int" .. outputdir .. "/%{prj.name}")

            pchheader "ikipch.h"
            pchsource "projects/%{prj.name}/src/ikipch.cpp"
            
            includedirs
            {
                "projects/Ikigai-Engine/src"
            }

            files
            {
                "projects/%{prj.name}/src/**.h",
                "projects/%{prj.name}/src/**.cpp",
            }

            filter "configurations:Debug"
                defines "IKIGAI_DEBUG_MODE"
                buildoptions "/MDd"
                symbols "on"

            filter "configurations:Release"
                defines "IKIGAI_RELEASE_MODE"
                buildoptions "/MD"
                optimize "on"
        
            filter "configurations:Distribution"
                defines "IKIGAI_DISTRIBUTION_MODE"
                buildoptions "/MD"
                optimize "on"
                
    group "Debug"
        project "Sandbox"
            location "projects/Sandbox"
            kind "ConsoleApp"
            language "C++"
            cppdialect "C++20"
            staticruntime "off"

            targetdir ("bin/" .. outputdir .. "/%{prj.name}")
            objdir ("bin/int" .. outputdir .. "/%{prj.name}")

            files
            {
                "projects/%{prj.name}/src/**.h",
                "projects/%{prj.name}/src/**.cpp",
            }

            includedirs
            {
                "projects/Ikigai-Engine/src"
            }        

            links
            {
                "Ikigai-Engine"
            }

            filter "configurations:Debug"
                buildoptions "/MDd"
                symbols "on"

            filter "configurations:Release"
                buildoptions "/MD"
                optimize "on"

            filter "configurations:Distribution"
                buildoptions "/MD"
                optimize "on"
            
        