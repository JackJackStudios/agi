#include "agipch.hpp"
#include "VulkanUtils.hpp"

namespace AGI {

	namespace Utils {

		bool ExtensionsSupported(const std::vector<const char*>& extensions)
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

		bool LayersSupported(const std::vector<const char*>& layers)
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

		bool IsDeviceSuitable(VkPhysicalDevice device, QueueFamilies* foundFamilies)
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
			}

			return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
				   deviceFeatures.geometryShader &&
				   foundFamilies->IsComplete();
		}

	}

}