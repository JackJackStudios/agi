#include "agipch.hpp"
#include "VulkanRenderContext.hpp"

namespace AGI {

	bool VulkanContext::CreateSwapchain(const glm::uvec2& size)
	{
		VkExtent2D extent = { size.x, size.y };

        int chosenIndex = 0;
        for (int i = 0; i < m_Device.SwapchainInfo.Formats.size(); ++i)
        {
            auto format = m_Device.SwapchainInfo.Formats[i];
            if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
            {
                chosenIndex = i;
                break;
            }
        }

        m_Swapchain.ImageFormat = m_Device.SwapchainInfo.Formats[chosenIndex];
        QuerySwapchainSupport(m_Device.Physical, &m_Device.SwapchainInfo);

        extent.width = glm::clamp(extent.width, (uint32_t)0, m_Device.SwapchainInfo.Capabilities.currentExtent.width);
        extent.height = glm::clamp(extent.height, (uint32_t)0, m_Device.SwapchainInfo.Capabilities.currentExtent.height);

        uint32_t images = m_Device.SwapchainInfo.Capabilities.minImageCount + 1;
        if (m_Device.SwapchainInfo.Capabilities.maxImageCount > 0 && images > m_Device.SwapchainInfo.Capabilities.maxImageCount)
        {
            images = m_Device.SwapchainInfo.Capabilities.maxImageCount;
        }

        // Swapchain create info
        VkSwapchainCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
        createInfo.surface = m_WindowSurface;
        createInfo.minImageCount = images;
        createInfo.imageFormat = m_Swapchain.ImageFormat.format;
        createInfo.imageColorSpace = m_Swapchain.ImageFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = { m_Device.QueueInfo.GraphicsIndex, m_Device.QueueInfo.PresentIndex };
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
        createInfo.queueFamilyIndexCount = 2;
        
        createInfo.preTransform = m_Device.SwapchainInfo.Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = 0;

        VK_CHECK_RETURN(vkCreateSwapchainKHR, m_Device.Logical, &createInfo, m_Allocator, &m_Swapchain.Handle);
        
        uint32_t image_count = 0;
        VK_CHECK_RETURN(vkGetSwapchainImagesKHR, m_Device.Logical, m_Swapchain.Handle, &image_count, 0);
        if (m_Swapchain.FramesInFlight != image_count) return false;

        m_Swapchain.Images.resize(m_Swapchain.FramesInFlight);
        m_Swapchain.ImageViews.resize(m_Swapchain.FramesInFlight);

        VK_CHECK_RETURN(vkGetSwapchainImagesKHR, m_Device.Logical, m_Swapchain.Handle, &image_count, m_Swapchain.Images.data());

        for (int i = 0; i < m_Swapchain.ImageViews.size(); ++i)
        {
            VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
            viewInfo.image = m_Swapchain.Images[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = m_Swapchain.ImageFormat.format;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            VK_CHECK_RETURN(vkCreateImageView, m_Device.Logical, &viewInfo, m_Allocator, &m_Swapchain.ImageViews[i]);
        }

		return true;
	}

	void VulkanContext::DestroySwapchain()
	{
        for (int i = 0; i < m_Swapchain.ImageViews.size(); ++i)
            vkDestroyImageView(m_Device.Logical, m_Swapchain.ImageViews[i], m_Allocator);

        vkDestroySwapchainKHR(m_Device.Logical, m_Swapchain.Handle, m_Allocator);
	}

};