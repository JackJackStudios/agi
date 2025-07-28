#include "agipch.hpp"
#include "VulkanRenderContext.hpp"

#include <magic_enum/magic_enum.hpp>

namespace AGI {

	void VulkanContext::Init()
	{
		////////////////////////////////////
		//////////    INSTANCE     /////////
		////////////////////////////////////

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "AGI Vulkan Instance";
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 3, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		appInfo.pEngineName = "AGI";

		auto validationLayers = GetRequiredExtensions(Extension::Validation);
		auto extensions = GetRequiredExtensions(Extension::Instance);

		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledLayerCount = validationLayers.size();
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
		instanceInfo.enabledExtensionCount = extensions.size();
		instanceInfo.ppEnabledExtensionNames = extensions.data();
		vkCreateInstance(&instanceInfo, nullptr, &m_Instance);

		////////////////////////////////////
		/////////  DEBUG MESSENGER  ////////
		////////////////////////////////////

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		vkCreateDebugMessenger(m_Instance, &createInfo, nullptr, &m_DebugMessenger);

		glfwCreateWindowSurface(m_Instance, m_BoundWindow->GetGlfwWindow(), nullptr, &m_WindowSurface);

		VkPhysicalDevice chosenDevice;
		if (!FindDevice(&chosenDevice))
		{
			AGI_ERROR("Failed to find a physical device that supports Vulkan");
		}

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(chosenDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(chosenDevice, &queueFamilyCount, queueFamilies.data());

		std::unordered_map<QueueType, uint32_t> foundQueues;
		for (int i = 0; i < queueFamilyCount; i++)
		{
			constexpr auto allQueues = magic_enum::enum_values<QueueType>();
			for (const auto& queue : allQueues)
			{
				if (queueFamilies[i].queueFlags & (VkQueueFlags)queue)
				{
					foundQueues[queue] = i;
				}
			}
		}

		std::vector<VkDeviceQueueCreateInfo> createInfos;
		for (const auto& [type, index] : foundQueues)
		{
			float queuePriority = 1.0f;

			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = index;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			createInfos.emplace_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo deviceInfo{};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount = createInfos.size();
		deviceInfo.pQueueCreateInfos = createInfos.data();
		deviceInfo.pEnabledFeatures = &deviceFeatures;

		auto validationLayers = GetRequiredExtensions(Extension::Validation);
		deviceInfo.enabledLayerCount = validationLayers.size();
		deviceInfo.ppEnabledLayerNames = validationLayers.data();

		vkCreateDevice(chosenDevice, &deviceInfo, nullptr, &m_Device);

		for (const auto& [type, index] : foundQueues)
		{
			m_Queues[type] = {};
			vkGetDeviceQueue(m_Device, index, 0, &m_Queues[type]);
		}
	}

	void VulkanContext::Shutdown()
	{
		vkDestroyDevice(m_Device, nullptr);
		vkDestroySurfaceKHR(m_Instance, m_WindowSurface, nullptr);

		vkDestroyDebugMessenger(m_Instance, m_DebugMessenger, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}


	bool VulkanContext::FindDevice(VkPhysicalDevice* device)
	{
		uint32_t deviceCount;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, NULL);

		if (deviceCount == 0)
			return false;

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

		for (int i = 0; i < deviceCount; i++)
		{
			/*
			VkPhysicalDeviceProperties properties;
			VkPhysicalDeviceFeatures features;
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceProperties(devices[i], &properties);
			vkGetPhysicalDeviceFeatures(devices[i], &features);
			vkGetPhysicalDeviceMemoryProperties(devices[i], &memProperties);
			*/

			uint32_t queueFamilyCount;
			vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, NULL);
			std::vector<VkQueueFamilyProperties> queuesFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, queuesFamilies.data());

			bool allQueuesFound = true;
			auto requiredQueues = GetRequiredExtensions(Extension::Queues);

			for (const auto& queue : requiredQueues)
			{
				bool found = false;
				for (uint32_t j = 0; j < queueFamilyCount; j++)
				{
					if ((queuesFamilies[j].queueFlags & (VkQueueFlags)magic_enum::enum_cast<QueueType>(queue).value()) != 0)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					allQueuesFound = false;
					break;
				}
			}

			if (!allQueuesFound)
				continue;


			bool extensionsSupported = true;

			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extensionCount, NULL);
			std::vector<VkExtensionProperties> deviceExt(extensionCount);
			vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extensionCount, deviceExt.data());

			auto requiredDeviceExt = GetRequiredExtensions(Extension::Device);
			for (const auto& ext : requiredDeviceExt)
			{
				bool found = false;
				for (uint32_t k = 0; k < extensionCount; k++)
					if (strcmp(ext, deviceExt[k].extensionName) == 0)
					{
						found = true;
						break;
					}

				if (!found)
				{
					extensionsSupported = false;
					break;
				}
			}

			if (!extensionsSupported)
				continue;

			uint32_t swapchainFormatCount, swapchainPresentModeCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], m_WindowSurface, &swapchainFormatCount, NULL);
			vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], m_WindowSurface, &swapchainPresentModeCount, NULL);

			if (swapchainFormatCount == 0 || swapchainPresentModeCount == 0)
				continue;

			*device = devices[i];
			return true;
		}

		return false;
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