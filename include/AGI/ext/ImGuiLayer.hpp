#pragma once

#include "agipch.hpp"
#include "AGI/Window.hpp"

#include <imgui.h>

namespace AGI {

    class ImGuiLayer
    {
    public:
        ImGuiLayer(Window* window);
        ~ImGuiLayer();

        void BeginFrame(bool dockspace = false);
        void EndFrame();

        static std::unique_ptr<ImGuiLayer> Create(Window* window) { return std::make_unique<ImGuiLayer>(window); }
    private:
        bool m_Dockspace;

        GLFWwindow* m_Window;
        ImGuiContext* m_Context; 
    };

};