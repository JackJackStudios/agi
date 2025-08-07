#include "utils.hpp"

int main(void)
{
    // Init spdlog for AGI callbacks
    InitLogging();

    AGI::Settings settings;
    settings.PreferedAPI = AGI::BestAPI();
    settings.MessageFunc = OnAGIMessage;
    settings.Blending = true;

    AGI::WindowProps windowProps;
    windowProps.Title = EXECUTABLE_NAME;
    windowProps.Size = { 720, 720 };

    /// This calles glfwCreateWindow but doesn't call glfwMakeContextCurrent
    auto window = AGI::Window::Create(settings, windowProps);
    
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
    delete context;
    
    return 0;
}