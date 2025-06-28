#pragma once

#include "agipch.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace AGI {

    class ImGuiLayer
    {
    public:
        ImGuiLayer(GLFWwindow* window, bool installCallbacks = true);
        ~ImGuiLayer();

        void BeginFrame(bool dockspace = false);
        void EndFrame();

        static std::shared_ptr<ImGuiLayer> Create(GLFWwindow* window) { return std::make_shared<ImGuiLayer>(window); }
    private:
        bool m_Dockspace;
    };

};