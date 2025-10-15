#include "utils.hpp"

int main(void)
{
    InitLogging();

    AGI::Settings settings;
    settings.PreferedAPI = AGI::APIType::Vulkan;
    settings.MessageFunc = OnAGIMessage;

    AGI::WindowProps windowProps;
    windowProps.Title = EXECUTABLE_NAME;
    windowProps.Size = { 720, 720 };

    auto window = AGI::Window::Create(settings, windowProps);
    auto context = AGI::RenderContext::Create(window);

    context->Init();

    while (!window->ShouldClose())
    {
        context->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        context->BeginFrame();

        context->EndFrame();
        window->PollEvents();
    }

    context->Shutdown();
    delete context;

    return 0;
}