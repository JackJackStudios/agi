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

	struct QueueFamilies
	{
		using size_type = int;
		size_type GraphicsFamily = -1;
		size_type PresentFamily = -1;

		bool IsComplete() const
		{
			return GraphicsFamily != -1 && PresentFamily != -1;
		}
	};
	
	constexpr const char* SDK_VALIDATION_LAYER = "VK_LAYER_KHRONOS_validation";
	constexpr const char* SWAPCHAIN_EXTENSION = "VK_KHR_swapchain";

	namespace Utils {

		static VkResult CreateDebugMessenger(
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

		static void DestroyDebugMessenger(
			VkInstance instance, 
			VkDebugUtilsMessengerEXT debugMessenger, 
			const VkAllocationCallbacks* pAllocator)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func == nullptr)
				return;

			func(instance, debugMessenger, pAllocator);
		}

		static bool ExtensionsSupported(const std::vector<const char*>& extensions)
		{
			uint32_t extensionCount;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

			for (const char* extName : extensions)
			{
				bool extFound = false;

				for (const auto& extProperties : availableExtensions)
				{
					if (strcmp(extName, extProperties.extensionName) == 0)
					{
						extFound = true;
						break;
					}
				}

				if (!extFound)
					return false;
			}

			return true;
		}

		static bool LayersSupported(const std::vector<const char*>& layers)
		{
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : layers)
			{
				bool layerFound = false;

				for (const auto& layerProperties : availableLayers)
				{
					if (strcmp(layerName, layerProperties.layerName) == 0) 
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound)
					return false;
			}

			return true;
		}

		static std::vector<const char*> GetRequiredExtensions()
		{
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
			extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

			if (!Utils::ExtensionsSupported(extensions))
			{
				AGI_ERROR("Extensions are not supported");
			}

			return extensions;
		}

		static std::vector<const char*> GetRequiredLayers()
		{
			std::vector<const char*> layers;
			layers.emplace_back(SDK_VALIDATION_LAYER);

			if (!Utils::LayersSupported(layers))
			{
				AGI_ERROR("Validation layers are not supported");
			}

			return layers;
		}

	}

}