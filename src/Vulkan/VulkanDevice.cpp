#include "agipch.hpp"
#include "VulkanRenderContext.hpp"

namespace AGI {

	bool VulkanContext::CreateDevice()
	{
		DeviceRequirements requirements = {};
		requirements.Graphics = true;
		requirements.Present = true;
		requirements.Transfer = true;
		requirements.DeviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		requirements.Extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		if (!MatchPhysicalDevice(&m_Device.Physical, requirements))
		{
			AGI_ERROR("No devices which requirements were found.");
			return false;
		}

		return true;
	}

	void VulkanContext::DestroyDevice()
	{

	}

	bool VulkanContext::MatchPhysicalDevice(VkPhysicalDevice* device, DeviceRequirements& requirements)
	{
		uint32_t physical_device_count = 0;
		VK_CHECK(vkEnumeratePhysicalDevices, m_Instance, &physical_device_count, 0);
		if (physical_device_count == 0) 
		{
			AGI_ERROR("No devices which support Vulkan were found.");
			return false;
		}

		std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
		VK_CHECK(vkEnumeratePhysicalDevices, m_Instance, &physical_device_count, physical_devices.data());

		for (const auto& device : physical_devices)
		{
			VkPhysicalDeviceProperties properties;
			VkPhysicalDeviceFeatures features;
			VkPhysicalDeviceMemoryProperties memory_props;
			vkGetPhysicalDeviceProperties(device, &properties);
			vkGetPhysicalDeviceFeatures(device, &features);
			vkGetPhysicalDeviceMemoryProperties(device, &memory_props);


		}

		return true;
	}

};