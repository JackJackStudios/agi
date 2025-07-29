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
			AGI_ERROR("No devices which matched requirements were found.");
			return false;
		}

		return true;
	}

	void VulkanContext::DestroyDevice()
	{

	}

	bool VulkanContext::MatchPhysicalDevice(VkPhysicalDevice* chosen_device, DeviceRequirements& requirements)
	{
		auto physical_devices = EnumerateParent<VkPhysicalDevice>(vkEnumeratePhysicalDevices, m_Instance);
		if (physical_devices.size() == 0)
		{
			AGI_ERROR("No devices which support Vulkan were found.");
			return false;
		}

		for (const auto& device : physical_devices)
		{
			VkPhysicalDeviceProperties properties;
			VkPhysicalDeviceFeatures features;
			VkPhysicalDeviceMemoryProperties memory;
			vkGetPhysicalDeviceProperties(device, &properties);
			vkGetPhysicalDeviceFeatures(device, &features);
			vkGetPhysicalDeviceMemoryProperties(device, &memory);

			bool result = DeviceMeetsRequirements(device, &m_Device.QueueInfo, &m_Device.SwapchainInfo, &requirements);
			if (result)
			{
				AGI_TRACE("Selected device: \"{}\" ({})", properties.deviceName, DeviceTypeToString(properties.deviceType));

				AGI_TRACE(
					"Vulkan API version: {}.{}.{}",
					VK_API_VERSION_MAJOR(properties.apiVersion),
					VK_API_VERSION_MINOR(properties.apiVersion),
					VK_API_VERSION_PATCH(properties.apiVersion));

				*chosen_device = device;
				break;
			}
		}

		if (*chosen_device == nullptr)
			return false;

		return true;
	}

	bool VulkanContext::DeviceMeetsRequirements(
		VkPhysicalDevice device,
		QueueIndexes* queue_info,
		SwapchainSupport* swapchain_support,
		const DeviceRequirements* requirements)
	{
		VkPhysicalDeviceProperties properties;
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceProperties(device, &properties);
		vkGetPhysicalDeviceFeatures(device, &features);

		if (properties.deviceType != requirements->DeviceType)
			return false;

		auto queue_families = EnumerateParent<VkQueueFamilyProperties>(vkGetPhysicalDeviceQueueFamilyProperties, device);

		// Look at each family and see which queues it supports
		uint8_t min_score = 255;
		for (int i = 0; i < queue_families.size(); i++)
		{
			uint8_t current_score = 0;

			if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				queue_info->GraphicsIndex = i;
				++current_score;
			}

			if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				queue_info->ComputeIndex = i;
				++current_score;
			}

			if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				if (current_score <= min_score)
				{
					min_score = current_score;
					queue_info->TransferIndex = i;
				}
			}

			VkBool32 present_supported = VK_FALSE;
			VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR, device, i, m_WindowSurface, &present_supported);
			if (present_supported) queue_info->PresentIndex = i;
		}

		if (
			(!requirements->Graphics || (requirements->Graphics && queue_info->GraphicsIndex != -1)) &&
			(!requirements->Present  || (requirements->Present  && queue_info->PresentIndex  != -1)) &&
			(!requirements->Compute  || (requirements->Compute  && queue_info->ComputeIndex  != -1)) &&
			(!requirements->Transfer || (requirements->Transfer && queue_info->TransferIndex != -1)))
		{
			QuerySwapchainSupport(device, swapchain_support);

			if (swapchain_support->Formats.empty() || swapchain_support->PresentModes.empty())
				return false;

			if (!requirements->Extensions.empty())
			{
				uint32_t extension_count = 0;
				vkEnumerateDeviceExtensionProperties(device, 0, &extension_count, 0);
				std::vector<VkExtensionProperties> available_extensions(extension_count);
				vkEnumerateDeviceExtensionProperties(device, 0, &extension_count, available_extensions.data());

				for (const auto& required_extension : requirements->Extensions)
				{
					bool found = false;
					for (const auto& extension : available_extensions)
					{
						if (strcmp(required_extension, extension.extensionName) == 0)
						{
							found = true;
							break;
						}
					}

					if (!found)
						return false;
				}
			}

			return true;
		}

		return false;
	}

	void VulkanContext::QuerySwapchainSupport(VkPhysicalDevice device, SwapchainSupport* swapchain_info)
	{
		// Surface capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_WindowSurface, &swapchain_info->Capabilities);

		// Surface formats
		swapchain_info->Formats = EnumerateParent<VkSurfaceFormatKHR>(vkGetPhysicalDeviceSurfaceFormatsKHR, device, m_WindowSurface);
		swapchain_info->PresentModes = EnumerateParent<VkPresentModeKHR>(vkGetPhysicalDeviceSurfacePresentModesKHR, device, m_WindowSurface);
	}

};