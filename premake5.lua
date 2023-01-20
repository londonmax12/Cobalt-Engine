outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Cobalt"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

    group "Core"
        project "Cobalt-Engine"
            location "projects/Cobalt-Engine"
            kind "StaticLib"
            language "C++"
            cppdialect "C++20"
            staticruntime "off"

            targetdir ("bin/" .. outputdir .. "/%{prj.name}")
            objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

            pchheader "cobaltpch.h"
            pchsource "projects/%{prj.name}/src/cobaltpch.cpp"
            
            VULKAN_SDK = os.getenv("VULKAN_SDK")

            LibraryDir = {}
        
            LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

            includedirs
            {
                "projects/Cobalt-Engine/src",
                "%{VULKAN_SDK}/Include"
            }

            files
            {
                "projects/%{prj.name}/src/**.h",
                "projects/%{prj.name}/src/**.cpp",
            }


            libdirs
            {
                "%{VULKAN_SDK}/Lib"
            }

            links
            {
                "vulkan-1.lib"
            }

            filter "configurations:Debug"
                defines "COBALT_DEBUG_MODE"
                buildoptions "/MDd"
                symbols "on"
                
                filter "configurations:Release"
                defines "COBALT_RELEASE_MODE"
                buildoptions "/MD"
                optimize "on"
                
                filter "configurations:Distribution"
                defines "COBALT_DISTRIBUTION_MODE"
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
            objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

            files
            {
                "projects/%{prj.name}/src/**.h",
                "projects/%{prj.name}/src/**.cpp",
            }

            includedirs
            {
                "projects/Cobalt-Engine/src"
            }        

            links
            {
                "Cobalt-Engine"
            }

            filter "configurations:Debug"
                defines "COBALT_DEBUG_MODE"
                buildoptions "/MDd"
                symbols "on"

            filter "configurations:Release"
                defines "COBALT_RELEASE_MODE"
                buildoptions "/MD"
                optimize "on"

            filter "configurations:Distribution"
                defines "COBALT_DISTRIBUTION_MODE"
                buildoptions "/MD"
                optimize "on"
            
        