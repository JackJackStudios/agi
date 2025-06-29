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

    /// This calles glfwCreateWindow but doesnt call glfwMakeContextCurrent
    auto window = AGI::Window::Create(settings);
    
    // This simplify creates the context in memory and nothing else
    auto context = AGI::RenderContext::Create(window);

    // This does every thread-specific initization and is the actual starting up of AGI
    // Anything after this call using the api should be in the same thread as it
    context->Init();

    // Main loop now
    while (!window->ShouldClose())
    {
        context->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        context->Clear();

        window->OnUpdate();
    }

    context->Shutdown();
    window.reset();
    return 0;
}