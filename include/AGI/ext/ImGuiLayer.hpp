#pragma once

#include "agipch.hpp"
#include "AGI/Window.hpp"

#include <imgui.h>

namespace AGI {

    class ImGuiLayer
    {
    public:
        ImGuiLayer(const std::unique_ptr<Window>& window, const ImGuiIO& config);
        ~ImGuiLayer();

        void BeginFrame(bool dockspace = false);
        void EndFrame();

        static std::shared_ptr<ImGuiLayer> Create(const std::unique_ptr<Window>& window, const ImGuiIO& config) { return std::make_shared<ImGuiLayer>(window, config); }
    private:
        bool m_Dockspace;
    };

};