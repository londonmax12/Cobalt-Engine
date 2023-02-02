#include "cobaltpch.h"

#include "VulkanMessenger.h"

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

bool Cobalt::VulkanMessenger::Init(Ref<VulkanState> state)
{
    m_State = state;
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debugCreateInfo.pfnUserCallback = VulkanDebugMessengerCallback;
    debugCreateInfo.pUserData = 0;

    PFN_vkCreateDebugUtilsMessengerEXT createFunction = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_State->Instance, "vkCreateDebugUtilsMessengerEXT");
    if (!createFunction)
    {
        COBALT_ERROR("Failed to create Vulkan debugger");
        return false;
    }
    if(!VK_CHECK(createFunction(m_State->Instance, &debugCreateInfo, m_State->Allocator, &m_Messenger)))
        return false;

    return true;
}

void Cobalt::VulkanMessenger::Shutdown()
{
    if (m_Messenger)
    {
        PFN_vkDestroyDebugUtilsMessengerEXT destroyFunction = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_State->Instance, "vkDestroyDebugUtilsMessengerEXT");
        destroyFunction(m_State->Instance, m_Messenger, m_State->Allocator);
        COBALT_DEBUG("Destroyed Vulkan messenger");
    }
}