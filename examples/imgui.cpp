#include "utils.hpp"

int main(void)
{
    // Init spdlog for AGI callbacks
    InitLogging();

    // Create GLFW window and the AGI::RenderContext
    AGI::Settings settings;
    settings.PreferedAPI = AGI::BestAPI();
    settings.MessageFunc = OnAGIMessage;
    settings.Blending = true;

    AGI::WindowProps windowProps;
    windowProps.Width = 720;
    windowProps.Height = 720;
    windowProps.Title = EXECUTABLE_NAME;
    
    windowProps.Maximise = true;

    auto window = AGI::Window::Create(settings, windowProps);
    auto context = AGI::RenderContext::Create(window);

    context->Init();

    auto imgui = AGI::ImGuiLayer::Create(window);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // Main loop now, you know the deal
    while (!window->ShouldClose())
    {
        context->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        context->Clear();

        imgui->BeginFrame();
        ImGui::ShowDemoWindow();
        imgui->EndFrame();

        window->OnUpdate();
    }

    imgui.reset();

    context->Shutdown();
    delete context;
    
    return 0;
}