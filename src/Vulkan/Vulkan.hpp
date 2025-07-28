#pragma once

#include <AGI/agi.hpp>
#include <vulkan/vulkan.h>

#define VK_CONCAT_IMPL(x, y) x##y
#define VK_CONCAT(x, y) VK_CONCAT_IMPL(x, y)
#define VK_CHECK(function, ...) \
    VkResult VK_CONCAT(function##_result_, __LINE__) = function(__VA_ARGS__); \
    if (VK_CONCAT(function##_result_, __LINE__) != VK_SUCCESS) \
    { \
        AGI_ERROR("{} failed with result: {}", #function, (int32_t)VK_CONCAT(function##_result_, __LINE__)); \
    }

template<typename T, VkResult(*EnumFunc)(uint32_t*, T*)>
std::vector<T> Enumerate()
{
    uint32_t count = 0;
    VK_CHECK(EnumFunc, &count, nullptr);

    std::vector<T> result;
    if (count > 0) {
        result.resize(count);
        VK_CHECK(EnumFunc, &count, result.data());
    }
    return result;
}

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