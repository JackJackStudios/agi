#pragma once

#include "agipch.hpp"

#include <AGI/Window.hpp>
#include <imgui.h>

namespace AGI {

    class ImGuiLayer
    {
    public:
        ImGuiLayer(std::unique_ptr<Window>& window, bool installCallbacks = true);
        ~ImGuiLayer();

        void BeginFrame(bool dockspace = false);
        void EndFrame();
    private:
        bool m_Dockspace;
    };

};