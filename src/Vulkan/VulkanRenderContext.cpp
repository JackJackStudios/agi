#include "agipch.hpp"
#include "VulkanRenderContext.hpp"

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
		appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 2, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		appInfo.pEngineName = "AGI";

		auto validationLayers = Utils::GetRequiredLayers();
		auto extensions = Utils::GetRequiredExtensions();

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
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr; // Optional
		Utils::CreateDebugMessenger(m_Instance, &createInfo, nullptr, &m_DebugMessenger);

		////////////////////////////////////
		////////// WINDOW SURFACE //////////
		////////////////////////////////////

		glfwCreateWindowSurface(m_Instance, m_BoundWindow->GetGlfwWindow(), nullptr, &m_WindowSurface);

		////////////////////////////////////
		////////// LOGICAL DEVICE //////////
		////////////////////////////////////

		VkPhysicalDevice physicalDevice = nullptr;
		QueueFamilies indices;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

		for (const auto& device : devices) 
		{
			memset(&indices, 0, sizeof(QueueFamilies));
			if (IsDeviceSuitable(device, &indices))
			{
				physicalDevice = device;

				CreateDevice(physicalDevice, indices);
				break;
			}
		}

		if (!physicalDevice)
		{
			AGI_ERROR("Cannot find GPUs with sufficient Vulkan support");
		}

	}

	void VulkanContext::Shutdown()
	{
		vkDestroyDevice(m_Device, nullptr);

		Utils::DestroyDebugMessenger(m_Instance, m_DebugMessenger, nullptr);

		vkDestroySurfaceKHR(m_Instance, m_WindowSurface, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}

	bool VulkanContext::IsDeviceSuitable(VkPhysicalDevice device, QueueFamilies* foundFamilies)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures; 
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		for (int i = 0; i < queueFamilyCount; i++)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				foundFamilies->GraphicsFamily = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_WindowSurface, &presentSupport);

			if (presentSupport)
				foundFamilies->PresentFamily = i;
		}

		return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
				deviceFeatures.geometryShader &&
				foundFamilies->IsComplete();
	}

	void VulkanContext::CreateDevice(VkPhysicalDevice physicalDevice, const QueueFamilies& wantedFamilies)
	{
		float queuePriority = 1.0f;
		std::vector<VkDeviceQueueCreateInfo> createInfos;
		std::vector<uint32_t> uniqueQueueFamilies;

		uniqueQueueFamilies.emplace_back((uint32_t)wantedFamilies.GraphicsFamily);
		uniqueQueueFamilies.emplace_back((uint32_t)wantedFamilies.PresentFamily);

		for (uint32_t familyIndex : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = familyIndex;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			createInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = createInfos.size();
		createInfo.pQueueCreateInfos = createInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;

		auto validationLayers = Utils::GetRequiredLayers();
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();

		vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_Device);

		vkGetDeviceQueue(m_Device, wantedFamilies.GraphicsFamily, 0, &m_GraphicsQuene);
		vkGetDeviceQueue(m_Device, wantedFamilies.PresentFamily, 0, &m_PresentQuene);
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