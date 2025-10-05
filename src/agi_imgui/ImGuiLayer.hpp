#pragma once

#include "agipch.hpp"
#include "AGI/Window.hpp"

#include <imgui/imgui.h>

namespace AGI {

    class ImGuiLayer
    {
    public:
        ImGuiLayer(RenderContext* context);
        ~ImGuiLayer();

        void BeginFrame(bool dockspace = false);
        void EndFrame();
    private:
        bool m_Dockspace;

        GLFWwindow* m_Window;
        ImGuiContext* m_Context; 
    };

};