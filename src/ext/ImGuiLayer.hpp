#pragma once

#include "agipch.hpp"
#include "AGI/Window.hpp"

#include <imgui.h>

namespace AGI {

    class ImGuiLayer
    {
    public:
        ImGuiLayer(RenderContext* context);
        ~ImGuiLayer();

        void BeginFrame(bool dockspace = false);
        void EndFrame();

        static std::unique_ptr<ImGuiLayer> Create(RenderContext* context) { return std::make_unique<ImGuiLayer>(context); }
    private:
        bool m_Dockspace;

        GLFWwindow* m_Window;
        ImGuiContext* m_Context; 
    };

};