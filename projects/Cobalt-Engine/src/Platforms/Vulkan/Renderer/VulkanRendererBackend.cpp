#include "cobaltpch.h"
#include "VulkanRendererBackend.h"

#include "Platforms/Vulkan/VulkanPlatform.h"
#include "Platforms/Vulkan/Utility/VulkanUtility.h"
#include "Platforms/Vulkan/Renderer/VulkanDevice.h"
#include "Platforms/Vulkan/Renderer/VulkanSwapchain.h"
#include "Platforms/Vulkan/Renderer/VulkanRenderPass.h"
#include "Platforms/Vulkan/Renderer/VulkanFence.h"

bool Cobalt::VulkanRendererBackend::Init(const char* applicationName, Ref<Platform::PlatformState> platformState, int width, int height)
{
    m_State = CreateRef<VulkanState>();
    m_State->FramebufferWidth = width;
    m_State->FramebufferHeight = height;

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
    instanceCreateInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
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

    instanceCreateInfo.enabledLayerCount = (uint32_t)requiredLayers.size();
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
    if (!m_State->Device->Init(m_State))
        return false;
    COBALT_INFO("Created Vulkan device");

    m_State->Swapchain = CreateRef<VulkanSwapchain>();
    if (!m_State->Swapchain->Init(m_State))
        return false;
    COBALT_INFO("Created Vulkan swapcahin");

    m_State->RenderPass = CreateRef<VulkanRenderPass>();
    if (!m_State->RenderPass->Init(m_State, { 0,0 }, { m_State->FramebufferWidth, m_State->FramebufferHeight }, { 0.8f,0.3f,0.2f, 1.f }, 1.f, 0))
        return false;
    COBALT_INFO("Created Vulkan render pass");

    m_State->Swapchain->RegenerateFramebuffers(m_State->RenderPass);

    if (!CreateCommandBuffers()) {
        COBALT_ERROR("Failed to create Vulkan command buffers");
        return false;
    }
    COBALT_INFO("Created Vulkan command buffers");

    if (!CreateSyncObjects())
    {
        COBALT_ERROR("Failed to create Vulkan sync objects");
        return false;
    }
    COBALT_INFO("Created Vulkan semaphores and fences");

    COBALT_INFO("Vulkan renderer backend initialized");

    return true;
}

void Cobalt::VulkanRendererBackend::Shutdown()
{
    vkDeviceWaitIdle(m_State->Device->GetLogicalDevice());

    for (int i = 0; i < m_State->Swapchain->GetMaxFramesInFlight(); i++) {
        if (m_State->ImageAvailableSemaphores[i]) {
            vkDestroySemaphore(m_State->Device->GetLogicalDevice(), m_State->ImageAvailableSemaphores[i], m_State->Allocator);
        }
        if (m_State->QueueCompleteSemaphores[i]) {
            vkDestroySemaphore(m_State->Device->GetLogicalDevice(), m_State->QueueCompleteSemaphores[i], m_State->Allocator);
        }
        m_State->InFlightFences[i].Shutdown();
    }
    COBALT_INFO("Destroyed Vulkan sync objects");

    m_State->RenderPass->Shutdown();
    COBALT_INFO("Destroyed Vulkan render pass");

    m_State->Swapchain->Shutdown();
    COBALT_INFO("Destroyed Vulkan swapchain");

    m_State->Device->Shutdown();
    COBALT_INFO("Destroyed Vulkan device");
       
    if (m_State->Surface) {
        vkDestroySurfaceKHR(m_State->Instance, m_State->Surface, m_State->Allocator);
        COBALT_INFO("Destroyed Vulkan surface");
    }

#ifdef COBALT_DEBUG_MODE
    m_Debugger->Shutdown();
    COBALT_DEBUG("Destroyed Vulkan messager");
#endif

    if (m_State->Instance)
        vkDestroyInstance(m_State->Instance, m_State->Allocator);

    COBALT_INFO("Destroyed Vulkan instance");
}

void Cobalt::VulkanRendererBackend::Resized(int width, int height)
{
    m_State->FramebufferWidth = width;
    m_State->FramebufferHeight = height;
}

bool Cobalt::VulkanRendererBackend::BeginFrame(DeltaTime dt)
{
    return true;
}

bool Cobalt::VulkanRendererBackend::EndFrame(DeltaTime dt)
{
    return true;
}

bool Cobalt::VulkanRendererBackend::CreateCommandBuffers()
{
    if (m_State->GraphicsCommandBuffers.size())
        return false;

    m_State->GraphicsCommandBuffers = std::vector<VulkanCommandBuffer>(m_State->Swapchain->GetImageCount());

    for (int i = 0; i < m_State->Swapchain->GetImageCount(); i++) {
        if (m_State->GraphicsCommandBuffers[i].GetCommandBuffer()) {
            m_State->GraphicsCommandBuffers[i].Free(m_State->Device->GetGraphicsCommandPool());
        }
        if (!m_State->GraphicsCommandBuffers[i].Allocate(m_State, m_State->Device->GetGraphicsCommandPool(), true))
            return false;
    }

    return true;
}

bool Cobalt::VulkanRendererBackend::CreateSyncObjects()
{
    int maxFramesInFlight = m_State->Swapchain->GetMaxFramesInFlight();
    m_State->ImageAvailableSemaphores = std::vector<VkSemaphore>(maxFramesInFlight);
    m_State->QueueCompleteSemaphores = std::vector<VkSemaphore>(maxFramesInFlight);
    m_State->InFlightFences = std::vector<VulkanFence>(maxFramesInFlight);

    for (int i = 0; i < maxFramesInFlight; i++) {
        VkSemaphoreCreateInfo semaphoreCreateInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        if (!VK_CHECK(vkCreateSemaphore(m_State->Device->GetLogicalDevice(),
            &semaphoreCreateInfo,
            m_State->Allocator,
            &m_State->ImageAvailableSemaphores[i])))
        {
            COBALT_ERROR("Failed to make image available semaphore at index: {}", i);
            return false;
        }
        if (!VK_CHECK(vkCreateSemaphore(m_State->Device->GetLogicalDevice(),
            &semaphoreCreateInfo,
            m_State->Allocator,
            &m_State->ImageAvailableSemaphores[i])))
        {
            COBALT_ERROR("Failed to make queue complete semaphore at index: {}", i);
            return false;
        }

        m_State->InFlightFences[i] = VulkanFence();
        if (!m_State->InFlightFences[i].Init(m_State, true)) {
            COBALT_ERROR("Failed to make image in flight fence at index: {}", i);
            return false;
        }
    }
}