#pragma once

#include "agipch.hpp"

#include "AGI/Window.hpp"

namespace AGI {

    class ImGuiLayer
    {
    public:
        ImGuiLayer(std::unique_ptr<Window>& window);
        ~ImGuiLayer();

        void BeginFrame(bool dockspace = false);
        void EndFrame();
    private:
        bool m_Dockspace;
    };

};