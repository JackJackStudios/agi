#include "utils.hpp"

int main(void)
{
    // Init spdlog for AGI callbacks
    InitLogging();

    // Create GLFW window and the AGI::RenderContext
    AGI::Settings settings;
    settings.PreferedAPI = AGI::APIType::Guess;
    settings.LoaderFunc = AGI::Window::LoaderFunc;
    settings.MessageFunc = OnAGIMessage;
    settings.Blending = true;

    AGI::WindowProps windowProps;
    windowProps.Width = 720;
    windowProps.Height = 720;
    windowProps.Title = EXECUTABLE_NAME;

    auto context = AGI::RenderContext::Create(settings);
    auto window = AGI::Window::Create(windowProps, context);
    AGI::ImGuiLayer* imgui = new AGI::ImGuiLayer(window);

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

    delete imgui;

    context->Shutdown();
    window.reset();
    return 0;
}