#include "utils.hpp"

void WindowLoop(AGI::RenderContext* context)
{
    auto window = context->GetBoundWindow();
    context->Init();
    
    while (!window->ShouldClose())
    {
        context->SetClearColour({ 1.0f, 0.0f, 0.0f, 1 });
        context->Clear();
        
        window->OnUpdate();
    }

    context->Shutdown();
}

int main(void)
{
    AGI::Settings settings;
    settings.PreferedAPI = AGI::BestAPI();
    settings.Blending = true;

    settings.Window.Width = 400;
    settings.Window.Height = 300;

    settings.Window.Title = "window1";
    auto window1 = AGI::Window::Create(settings);
    auto context1 = AGI::RenderContext::Create(window1);

    settings.Window.Title = "window2";
    auto window2 = AGI::Window::Create(settings);
    auto context2 = AGI::RenderContext::Create(window2);

    std::thread thread(WindowLoop, context2);
    WindowLoop(context1);

    thread.join();

    delete context1;
    delete context2;
    return 0;
}
