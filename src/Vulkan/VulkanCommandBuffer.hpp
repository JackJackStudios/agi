#pragma once
#include "Vulkan.hpp"

namespace AGI {

    class VulkanContext;

    class VulkanCommandBuffer
    {
    public:
        enum class State
        {
            Ready = 0, Recording, InPass, RecordingEnded, Submitted, NotAllocated,
        };
        VulkanCommandBuffer() = default;

        bool Allocate(VulkanContext* context, VkCommandPool* pool, bool is_primary);
        void Free();

        // Setters and getters
        VkCommandBuffer GetHandle() const { return m_RendererID; }
        State GetState() const { return m_CurrentState; }

        // Changes state
        void Begin(bool single_use, bool is_renderpass, bool simultaneous);
        void End();
        
        void UpdateSubmitted();
        void Reset();
    private:
        VulkanContext* m_BoundContext = nullptr;
        VkCommandPool* m_Parent = nullptr;

        VkCommandBuffer m_RendererID = nullptr;
        State m_CurrentState = State::NotAllocated;
    };

};