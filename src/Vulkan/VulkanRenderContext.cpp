#include "agipch.hpp"
#include "VulkanRenderContext.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	switch (messageSeverity)
	{
	default:
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

	bool VulkanContext::Init()
	{
		VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
		app_info.apiVersion = VK_API_VERSION_1_2;
		app_info.pApplicationName = m_BoundWindow->GetTitle().c_str();
		app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		app_info.engineVersion = VK_MAKE_API_VERSION(0, 0, 3, 0);
		app_info.pEngineName = "AGI";

		VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		create_info.pApplicationInfo = &app_info;
		
		std::vector<const char*> required_extensions;
		std::vector<const char*> required_validation;
		
		// Get extensions required by glfw
		uint32_t extensionCount;
		const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

		for (int i = 0; i < extensionCount; i++)
			required_extensions.emplace_back(extensions[i]);

		if (m_Settings.EnableValidation)
		{
			required_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
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
					return false;
				}
			}
		}

		create_info.enabledExtensionCount = required_extensions.size();
		create_info.ppEnabledExtensionNames = required_extensions.data();
		create_info.enabledLayerCount = required_validation.size();
		create_info.ppEnabledLayerNames = required_validation.data();

		VK_CHECK_RETURN(vkCreateInstance, &create_info, m_Allocator, &m_Instance);

		if (m_Settings.EnableValidation)
		{
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
		}
		
		m_BoundWindow->Init();

		if (glfwCreateWindowSurface(m_Instance, m_BoundWindow->GetGlfwWindow(), m_Allocator, &m_WindowSurface) != VK_SUCCESS)
		{
			AGI_ERROR("Failed to create Vulkan surface");
			return false;
		}

		if (!CreateDevice())
		{
			AGI_ERROR("Failed to create Vulkan logical device");
			return false;
		}

		if (!CreateSwapchain(m_BoundWindow->GetSize()))
		{
			AGI_ERROR("Failed to create Vulkan swapchain");
			return false;
		}

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(m_Device.Physical, &props);
		m_Properties.Renderer = std::format("{} ({})", props.deviceName, DeviceTypeToString(props.deviceType));
		m_Properties.Version = std::format("{}.{}.{}", VK_API_VERSION_MAJOR(props.apiVersion), VK_API_VERSION_MINOR(props.apiVersion), VK_API_VERSION_PATCH(props.apiVersion));

		RenderPassSpecification spec;
		spec.RenderArea.z = m_BoundWindow->GetSize().x;
		spec.RenderArea.w = m_BoundWindow->GetSize().y;
		spec.ColourFormat = m_Swapchain.ImageFormat.format;

		if (!m_MainRenderpass.Create(this, spec))
		{
			AGI_ERROR("Failed to create Vulkan render pass");
			return false;
		}

		for (int i = 0; i < m_Swapchain.FramesInFlight; ++i)
		{
			// Create framebuffers
			VulkanFramebuffer& frambuffer = m_Swapchain.Framebuffers.emplace_back();
			frambuffer.Create(this, &m_MainRenderpass, m_BoundWindow->GetSize(), { m_Swapchain.ImageViews[i] });

			// Create command buffers
			VulkanCommandBuffer& buf = m_GraphicsCommands.emplace_back();
			buf.Allocate(this, m_Device.GraphicsPool, true);

			// Create sync objects
			VkSemaphoreCreateInfo createInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
			vkCreateSemaphore(m_Device.Logical, &createInfo, m_Allocator, &m_ImageAvailableSemaphores.emplace_back());
			vkCreateSemaphore(m_Device.Logical, &createInfo, m_Allocator, &m_QueueCompleteSemaphores.emplace_back());

			VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
			createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			VkFence& fence = m_InFlightFences.emplace_back();
			VK_CHECK_RETURN(vkCreateFence, m_Device.Logical, &fenceInfo, m_Allocator, &fence);
		}

		m_ImagesInFlight.resize(m_Swapchain.Images.size());

		PrintProperties();
		return true;
	}

	void VulkanContext::Shutdown()
	{
		vkDeviceWaitIdle(m_Device.Logical);

		for (int i = 0; i < m_Swapchain.FramesInFlight; ++i)
		{
			vkDestroySemaphore(m_Device.Logical, m_ImageAvailableSemaphores[i], m_Allocator);
			vkDestroySemaphore(m_Device.Logical, m_QueueCompleteSemaphores[i], m_Allocator);

			vkDestroyFence(m_Device.Logical, m_InFlightFences[i], m_Allocator);
		}

		for (int i = 0; i < m_GraphicsCommands.size(); ++i)
			m_GraphicsCommands[i].Free();

		for (int i = 0; i < m_Swapchain.Framebuffers.size(); ++i)
			m_Swapchain.Framebuffers[i].Destroy();

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
		AGI_WARN("TODO: Recreate swapchain");
	}

	void VulkanContext::SetClearColour(const glm::vec4& colour)
	{
		m_ClearColour = colour;
	}

	void VulkanContext::BeginFrame()
	{
		// 1) Wait for the frame we’re about to use to be idle (GPU done with it)
		vkWaitForFences(m_Device.Logical, 1, &m_InFlightFences[m_CurrentFrame], true, UINT64_MAX);
		vkResetFences(m_Device.Logical, 1, &m_InFlightFences[m_CurrentFrame]); // reset right after a successful wait

		// 2) Acquire next swapchain image (signal the per-frame "imageAvailable")
		AcquireNextImage(
			UINT64_MAX,
			m_ImageAvailableSemaphores[m_CurrentFrame],
			VK_NULL_HANDLE,
			&m_ImageIndex
		);

		// 3) If that image is already in flight, wait for whichever frame used it last
		if (m_InFlightFences[m_CurrentFrame] != VK_NULL_HANDLE)
			vkWaitForFences(m_Device.Logical, 1, &m_InFlightFences[m_CurrentFrame], true, UINT64_MAX);

		// Associate this image with the fence of the frame we're submitting now
		m_ImagesInFlight[m_ImageIndex] = m_InFlightFences[m_CurrentFrame];

		// 4) Build commands for this image
		VulkanCommandBuffer& commands = m_GraphicsCommands[m_ImageIndex];
		commands.Reset();
		commands.Begin(false, false, false);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = (float)m_BoundWindow->GetSize().y;
		viewport.width = (float)m_BoundWindow->GetSize().x;
		viewport.height = -(float)m_BoundWindow->GetSize().y; // flip Y (fine)
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { m_BoundWindow->GetSize().x, m_BoundWindow->GetSize().y };

		vkCmdSetViewport(commands.GetHandle(), 0, 1, &viewport);
		vkCmdSetScissor(commands.GetHandle(), 0, 1, &scissor);

		m_MainRenderpass.Begin(commands, m_ClearColour, m_Swapchain.Framebuffers[m_ImageIndex].GetHandle());
	}

	void VulkanContext::EndFrame()
	{
		m_MainRenderpass.End();

		VulkanCommandBuffer& commands = m_GraphicsCommands[m_ImageIndex];
		commands.End();

		// 5) Submit once, waiting on "imageAvailable", signaling "renderFinished"
		VkCommandBuffer cmd = commands.GetHandle();

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
		submitInfo.waitSemaphoreCount   = 1;
		submitInfo.pWaitSemaphores      = &m_ImageAvailableSemaphores[m_CurrentFrame];
		submitInfo.pWaitDstStageMask    = waitStages;
		submitInfo.commandBufferCount   = 1;
		submitInfo.pCommandBuffers      = &cmd;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores    = &m_QueueCompleteSemaphores[m_CurrentFrame];

		VK_CHECK(vkQueueSubmit,
			m_Device.GraphicsQueue,
			1,
			&submitInfo,
			m_InFlightFences[m_CurrentFrame] // this fence will be signaled when GPU finishes
		);

		// 6) Present waits on the render-finished semaphore
		PresentSwapchain(m_QueueCompleteSemaphores[m_CurrentFrame], m_ImageIndex);

		// 7) Advance frame index
		m_CurrentFrame = (m_CurrentFrame + 1) % m_Swapchain.FramesInFlight;
	}

	void VulkanContext::DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount)
	{
	}

}