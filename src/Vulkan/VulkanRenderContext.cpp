#include "agipch.hpp"
#include "VulkanRenderContext.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
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

	void VulkanContext::Init()
	{
		VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
		app_info.apiVersion = VK_API_VERSION_1_2;
		app_info.pApplicationName = m_BoundWindow->GetProperties().Title.c_str();
		app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		app_info.engineVersion = VK_MAKE_API_VERSION(0, 0, 3, 0);
		app_info.pEngineName = "AGI";

		VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		create_info.pApplicationInfo = &app_info;
		
		std::vector<const char*> required_extensions;
		std::vector<const char*> required_validation;
		
		// Get extensions required by glfw
		m_BoundWindow->GetVulkanExtensions(&required_extensions);

		// If in debug mode enable validation stuff.
#ifdef AGI_DEBUG
		AGI_TRACE("Vulkan validation is enabled.");

		required_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		
		//required_validation.emplace_back("VK_LAYER_LUNARG_api_dump");
		required_validation.emplace_back("VK_LAYER_KHRONOS_validation");

		auto available_layers = Enumerate<VkLayerProperties, vkEnumerateInstanceLayerProperties>();

		// Verify that layers exists
		for (const auto& required_layer : required_validation)
		{
			bool found = false;
			for (const auto& available_layer : available_layers)
			{
				if (strcmp(required_layer, available_layer.layerName) == 0)
				{
					found = true;
					break;
				}
			}

			if (!found) 
			{
				AGI_ERROR("Required validation layer is missing: {}", required_layer);
			}
		}
#endif

		create_info.enabledExtensionCount = required_extensions.size();
		create_info.ppEnabledExtensionNames = required_extensions.data();
		create_info.enabledLayerCount = required_validation.size();
		create_info.ppEnabledLayerNames = required_validation.data();

		VK_CHECK(vkCreateInstance, &create_info, m_Allocator, &m_Instance);

		// NOTE: Whetever this should be filtered out of release build
		//       is the user's decision.
		std::string extensions_str;
		for (const auto& ext : required_extensions)
			extensions_str.append(" ").append(ext);

#ifdef AGI_DEBUG
		for (const auto& val : required_validation)
			extensions_str.append(" ").append(val);
#endif

		AGI_TRACE("Vulkan extensions:{}", extensions_str);

#ifdef AGI_DEBUG
		// Create debugging
		uint32_t log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
			                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |  
		                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;

		VkDebugUtilsMessengerCreateInfoEXT debug_create_info = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
		debug_create_info.messageSeverity = log_severity;
		debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debug_create_info.pfnUserCallback = vk_debug_callback;

		VK_CHECK(vkCreateDebugMessenger, m_Instance, &debug_create_info, m_Allocator, &m_Debugger);
#endif
		
		m_BoundWindow->Init();

		if (glfwCreateWindowSurface(m_Instance, m_BoundWindow->GetGlfwWindow(), m_Allocator, &m_WindowSurface) != VK_SUCCESS)
			AGI_ERROR("Failed to create Vulkan surface");

		if (!CreateDevice())
			AGI_ERROR("Failed to create Vulkan logical device");

		if (!CreateSwapchain({ m_BoundWindow->GetWidth(), m_BoundWindow->GetHeight() }))
			AGI_ERROR("Failed to create Vulkan swapchain");

		RenderPassSpecification spec;
		spec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		spec.RenderArea.y = m_BoundWindow->GetWidth();
		spec.RenderArea.w = m_BoundWindow->GetHeight();
		spec.ColourFormat = m_Swapchain.ImageFormat.format;

		if (!m_MainRenderpass.Create(this, spec))
			AGI_ERROR("Failed to create Vulkan render pass");
	}

	void VulkanContext::Shutdown()
	{
		m_MainRenderpass.Destroy();

		DestroySwapchain();
		DestroyDevice();

		vkDestroySurfaceKHR(m_Instance, m_WindowSurface, m_Allocator);
#ifdef AGI_DEBUG
		vkDestroyDebugMessenger(m_Instance, m_Debugger, m_Allocator);
#endif
		vkDestroyInstance(m_Instance, m_Allocator);
	}

	void VulkanContext::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		// TODO: Recreate swapchain
	}

	void VulkanContext::SetClearColour(const glm::vec4& colour)
	{
		m_MainRenderpass.SetClearColour(colour);
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