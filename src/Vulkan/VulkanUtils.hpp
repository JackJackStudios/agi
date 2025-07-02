#pragma once 

#include <vulkan/vulkan.h>

namespace AGI {

	namespace Utils {

		bool ExtensionsSupported(const std::vector<const char*>& extensions);
		bool LayersSupported(const std::vector<const char*>& layers);

		bool IsDeviceSuitable(VkPhysicalDevice device);

	}

}