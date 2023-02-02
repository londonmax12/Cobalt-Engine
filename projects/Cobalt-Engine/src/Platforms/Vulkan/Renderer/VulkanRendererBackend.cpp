#include "cobaltpch.h"
#include "VulkanRendererBackend.h"

#include "Platforms/Vulkan/Utility/VulkanUtility.h"
#include "Platforms/Vulkan/VulkanPlatform.h"

bool Cobalt::VulkanRendererBackend::Init(const char* applicationName, Platform::PlatformState* platformState, int width, int height)
{
    m_State = CreateRef<VulkanState>();

    VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
    appInfo.apiVersion = VK_API_VERSION_1_3;
    appInfo.pApplicationName = applicationName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Cobalt Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    std::vector<const char*> requiredExtensions;
    requiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    Platform::GetVulkanExtensions(&requiredExtensions);

#ifdef COBALT_DEBUG_MODE
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    COBALT_DEBUG("Vulkan Required Extensions ({}):", requiredExtensions.size());
    for (auto& extension : requiredExtensions) {
        COBALT_DEBUG("\t| {}", extension);
    }
#endif

    VkInstanceCreateInfo instanceCreateInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledExtensionCount = requiredExtensions.size();
    instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.ppEnabledLayerNames = 0;

#ifdef COBALT_DEBUG_MODE
    std::vector<const char*> requiredLayers;
    requiredLayers.push_back("VK_LAYER_KHRONOS_validation");

    unsigned int availableLayerCount = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, 0));
    std::vector<VkLayerProperties> layerProperties(availableLayerCount);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, layerProperties.data()));

    for (auto& i : requiredLayers) {
        bool foundLayer = false;
        for (auto& j : layerProperties) {
            if (i = j.layerName)
            {
                foundLayer = true;
                break;
            }
        }
        if (!foundLayer) {
            COBALT_FATAL("Failed to find validation layer: {}", i);
            return false;
        }
    }
    COBALT_DEBUG("All required validation layers are present");

    instanceCreateInfo.enabledLayerCount = requiredLayers.size();
    instanceCreateInfo.ppEnabledLayerNames = requiredLayers.data();
#endif

    if (!VK_CHECK(vkCreateInstance(&instanceCreateInfo, m_State->Allocator, &m_State->Instance))) {
        COBALT_ERROR("Failed to initialize Vulkan instance");
        return false;
    }

#ifdef COBALT_DEBUG_MODE
    m_Debugger = CreateRef<VulkanMessenger>();
    if (!m_Debugger->Init(m_State))
        return false;
    COBALT_DEBUG("Created Vulkan debugger");
#endif

    m_State->Surface = Platform::CreateVulkanSurface(platformState, m_State);
    if (!m_State->Surface)
        return false;
    COBALT_INFO("Created Vulkan surface");

    m_State->Device = CreateRef<VulkanDevice>();
    if (!m_State->Device->Init(m_State->Instance, m_State->Surface, m_State->Allocator))
        return false;
    COBALT_INFO("Created Vulkan device");

    COBALT_INFO("Vulkan renderer backend initialized");

    return true;
}

void Cobalt::VulkanRendererBackend::Shutdown()
{
#ifdef COBALT_DEBUG_MODE
    m_Debugger->Shutdown();
#endif

    if (m_State->Instance)
        vkDestroyInstance(m_State->Instance, m_State->Allocator);

    COBALT_INFO("Destroyed Vulkan instance");
}

void Cobalt::VulkanRendererBackend::Resized(int width, int height)
{
}

bool Cobalt::VulkanRendererBackend::BeginFrame(DeltaTime dt)
{
    return true;
}

bool Cobalt::VulkanRendererBackend::EndFrame(DeltaTime dt)
{
    return true;
}