#include "agipch.hpp"
#include "VulkanRenderContext.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
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

	namespace Utils {

		VkResult CreateDebugMessenger(
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

		void DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func == nullptr)
				return;

			func(instance, debugMessenger, pAllocator);
		}

	}

	void VulkanContext::Init()
	{
		////////////////////////////////////
		//////////    INSTANCE     /////////
		////////////////////////////////////

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "AGI Vulkan Instance";
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.pEngineName = "AGI";
		appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 2, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		auto validationLayers = GetRequiredLayers();
		auto extensions = GetRequiredExtensions();

		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledLayerCount = validationLayers.size();
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
		instanceInfo.enabledExtensionCount = extensions.size();
		instanceInfo.ppEnabledExtensionNames = extensions.data();
		vkCreateInstance(&instanceInfo, nullptr, &m_Instance);

		////////////////////////////////////
		////////// DEBUG MESSENGER /////////
		////////////////////////////////////

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr; // Optional
		Utils::CreateDebugMessenger(m_Instance, &createInfo, nullptr, &m_DebugMessenger);

		////////////////////////////////////
		////////// PHYSICAL DEVICE /////////
		////////////////////////////////////

		VkPhysicalDevice physicalDevice = nullptr;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

		for (const auto& device : devices) 
		{
			if (Utils::IsDeviceSuitable(device)) 
			{
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == nullptr)
		{
			AGI_ERROR("Cannot find suitable GPU");
		}

	}

	void VulkanContext::Shutdown()
	{
		Utils::DestroyDebugMessenger(m_Instance, m_DebugMessenger, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}

	std::vector<const char*> VulkanContext::GetRequiredExtensions()
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

	std::vector<const char*> VulkanContext::GetRequiredLayers()
	{
		std::vector<const char*> layers;
		layers.emplace_back(SDK_VALIDATION_LAYER);

		if (!Utils::LayersSupported(layers))
		{
			AGI_ERROR("Validation layers are not supported");
		}

		return layers;
	}

	void VulkanContext::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
	}

	void VulkanContext::SetClearColour(const glm::vec4& colour)
	{
	}

	void VulkanContext::SetTextureAlignment(int align)
	{
	}

	void VulkanContext::Clear()
	{
	}

	void VulkanContext::DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount)
	{
	}

}