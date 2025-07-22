#include "utils.hpp"

void WindowLoop(AGI::RenderContext* context)
{
    auto window = context->GetBoundWindow();
    context->Init();
    
    while (!window->ShouldClose())
    {
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

    std::thread thread(WindowLoop, context2.get());
    WindowLoop(context1.get());

    thread.join();
    return 0;
}
