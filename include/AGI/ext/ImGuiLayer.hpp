#pragma once

#include "agipch.hpp"
#include "AGI/Window.hpp"

#include <imgui.h>

namespace AGI {

    class ImGuiLayer
    {
    public:
        ImGuiLayer(const std::unique_ptr<Window>& window);
        ~ImGuiLayer();

        void BeginFrame(bool dockspace = false);
        void EndFrame();

        static std::shared_ptr<ImGuiLayer> Create(const std::unique_ptr<Window>& window) { return std::make_shared<ImGuiLayer>(window); }
    private:
        bool m_Dockspace;
    };

};