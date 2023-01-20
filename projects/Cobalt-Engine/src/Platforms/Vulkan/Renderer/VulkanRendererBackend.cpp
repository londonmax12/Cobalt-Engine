#include "cobaltpch.h"
#include "VulkanRendererBackend.h"

#include "Platforms/Vulkan/Utility/VulkanUtility.h"

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
    switch (messageSeverity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        COBALT_TRACE(callbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        COBALT_INFO(callbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        COBALT_WARN(callbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        COBALT_ERROR(callbackData->pMessage);
        break;
    default:
        break;
    }
    return VK_FALSE;
}

bool Cobalt::VulkanRendererBackend::Init(const char* applicationName, Platform::PlatformState* platformState, int width, int height)
{
    VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
    appInfo.apiVersion = VK_API_VERSION_1_3;
    appInfo.pApplicationName = applicationName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Cobalt Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    std::vector<const char*> requiredExtensions;

    // Generic extensions
    requiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

    // Platform specific extensions
    Platform::GetVulkanExtensions(requiredExtensions);

    // Debug extensions
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
    COBALT_DEBUG("Vulkan validation layers enabled");
    
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

    if (!VK_CHECK(vkCreateInstance(&instanceCreateInfo, m_Allocator, &m_Instance))) {
        COBALT_ERROR("Failed to initialize Vulkan instance");
        return false;
    }

#ifdef COBALT_DEBUG_MODE
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debugCreateInfo.pfnUserCallback = VulkanDebugMessengerCallback;
    debugCreateInfo.pUserData = 0;

    PFN_vkCreateDebugUtilsMessengerEXT createFunction = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
    if (!createFunction)
    {
        COBALT_ERROR("Failed to create Vulkan debugger");
        return false;
    }
    VK_CHECK(createFunction(m_Instance, &debugCreateInfo, m_Allocator, &m_Messenger));

    COBALT_INFO("Created Vulkan debugger");
#endif

    COBALT_INFO("Vulkan renderer backend initialized");
    return true;


}

void Cobalt::VulkanRendererBackend::Shutdown()
{
#ifdef COBALT_DEBUG_MODE
    if (m_Messenger)
    {
        PFN_vkDestroyDebugUtilsMessengerEXT destroyFunction = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
        destroyFunction(m_Instance, m_Messenger, m_Allocator);
        COBALT_DEBUG("Destroying Vulkan messenger");
    }
#endif

    vkDestroyInstance(m_Instance, m_Allocator);
    COBALT_INFO("Destroyed Vulkan index");
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
