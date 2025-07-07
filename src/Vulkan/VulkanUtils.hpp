#pragma once 

#include <vulkan/vulkan.h>

namespace AGI {

	struct QueueFamilies
	{
		int GraphicsFamily = -1;

		bool IsComplete() const
		{
			return GraphicsFamily != -1;
		}
	};

	namespace Utils {

		bool ExtensionsSupported(const std::vector<const char*>& extensions);
		bool LayersSupported(const std::vector<const char*>& layers);

		bool IsDeviceSuitable(VkPhysicalDevice device, QueueFamilies* foundFamilies);

	}

}