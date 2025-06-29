#include "utils.hpp"

int main(void)
{
    // Init spdlog for AGI callbacks
    InitLogging();

    // Create GLFW window and the AGI::RenderContext
    AGI::Settings settings;
    settings.PreferedAPI = AGI::APIType::Guess;
    settings.MessageFunc = OnAGIMessage;
    settings.Blending = true;

    settings.Window.Width = 720;
    settings.Window.Height = 720;
    settings.Window.Title = EXECUTABLE_NAME;

    auto window = AGI::Window::Create(settings);
    auto context = AGI::RenderContext::Create(window);

    auto imgui = AGI::ImGuiLayer::Create(window);

    context->Init();
    
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
    window.reset();
    return 0;
}