#include "AGI/ext/ImGuiLayer.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

enum GlfwClientApi
{
	GlfwClientApi_Unknown,
	GlfwClientApi_OpenGL,
	GlfwClientApi_Vulkan,
};
static bool ImGui_ImplGlfw_Init(GLFWwindow* window, bool install_callbacks, GlfwClientApi client_api);

namespace AGI {

	namespace Utils {

		GlfwClientApi AgiTypeToImGuiType(APIType type)
		{
			switch (type)
			{
			case APIType::OpenGL: return GlfwClientApi_OpenGL;
			case APIType::Vulkan: return GlfwClientApi_Vulkan;

			case APIType::Guess: AGI_VERIFY(false, "APIType::Guess should not reach this function"); break;
			default: AGI_VERIFY(false, "Undefined APIType"); return GlfwClientApi_Unknown; break;
			}
			
			return GlfwClientApi_Unknown;
		}

	}

    ImGuiLayer::ImGuiLayer(const std::unique_ptr<Window>& window)
    {
        // Setup Dear ImGui context
		ImGui::CreateContext();

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window->GetGlfwWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
    }

    ImGuiLayer::~ImGuiLayer()
    {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
    }

	void ImGuiLayer::BeginFrame(bool dockspace)
	{
        m_Dockspace = dockspace;
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		if (dockspace)
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace", 0, window_flags);

			ImGui::PopStyleVar(3);

			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
		}
	}

    void ImGuiLayer::EndFrame()
    {
		if (m_Dockspace)
			ImGui::End();

		ImGuiIO& io = ImGui::GetIO();
        int width, height;

        glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
        io.DisplaySize = ImVec2((float)width, (float)height);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(glfwGetCurrentContext());
        }
    }

}