#pragma once 

#include <vulkan/vulkan.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		AGI::Log::GenericLog(pCallbackData->pMessage, AGI::LogLevel::Trace);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		AGI::Log::GenericLog(pCallbackData->pMessage, AGI::LogLevel::Info);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		AGI::Log::GenericLog(pCallbackData->pMessage, AGI::LogLevel::Warning);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		AGI::Log::GenericLog(pCallbackData->pMessage, AGI::LogLevel::Error);
		break;
	}

	return VK_FALSE;
}

namespace AGI {

	enum class Extension : uint8_t
	{
		Validation = 0,
		Device,
		Instance,
		Queues
	};

	static std::vector<std::pair<Extension, std::string_view>> s_RequiredExtensions =
	{
		// Validation layer
		{ Extension::Validation, "VK_LAYER_KHRONOS_validation" },
		{ Extension::Instance,   "VK_EXT_debug_utils" },

		{ Extension::Device,     "VK_KHR_swapchain" },
		{ Extension::Queues,     "Graphics" },
	};

	static VkResult vkCreateDebugMessenger(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func == nullptr)
			return VK_ERROR_EXTENSION_NOT_PRESENT;

		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}

	static void vkDestroyDebugMessenger(
		VkInstance instance, 
		VkDebugUtilsMessengerEXT debugMessenger, 
		const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func == nullptr)
			return;

		func(instance, debugMessenger, pAllocator);
	}

	static std::vector<const char*> GetRequiredExtensions(Extension type)
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> vec;
		for (const auto& [ext, name] : s_RequiredExtensions)
		{
			if (ext == type) vec.emplace_back(name.data());
		}

		if (type == Extension::Instance)
		{
			for (uint32_t i = 0; i < glfwExtensionCount; i++)
				vec.emplace_back(glfwExtensions[i]);
		}

		return vec;
	}

}