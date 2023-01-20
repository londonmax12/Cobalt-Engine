#include "cobaltpch.h"
#include "VulkanRendererBackend.h"

bool Cobalt::VulkanRendererBackend::Init(const char* applicationName, Platform::PlatformState* platformState, int width, int height)
{
    VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
    appInfo.apiVersion = VK_API_VERSION_1_3;
    appInfo.pApplicationName = applicationName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Cobalt Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instanceCreateInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledExtensionCount = 0;
    instanceCreateInfo.ppEnabledExtensionNames = 0;
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.ppEnabledLayerNames = 0;

    VkResult result = vkCreateInstance(&instanceCreateInfo, m_Allocator, &m_Instance);
    if (result != VK_SUCCESS) {
        COBALT_ERROR("Failed to initialize Vulkan instance");
        return false;
    }

    COBALT_INFO("Vulkan renderer initialized");
    return true;
}

void Cobalt::VulkanRendererBackend::Shutdown()
{
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
