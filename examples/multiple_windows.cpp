#include "utils.hpp"

void WindowLoop(AGI::RenderContext* context)
{
    auto window = context->GetBoundWindow();
    context->Init();

    auto imgui = AGI::ImGuiLayer::Create(context);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    while (!window->ShouldClose())
    {
        context->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        context->Clear();

        imgui->BeginFrame();
        ImGui::ShowAboutWindow();
        imgui->EndFrame();

        window->OnUpdate();
    }

    imgui.reset();
    context->Shutdown();
}

int main(void)
{
    InitLogging();

    AGI::Settings settings;
    settings.PreferedAPI = AGI::BestAPI();
    settings.MessageFunc = OnAGIMessage;
    settings.Blending = true;

    AGI::WindowProps windowProps;
    windowProps.Size = { 400, 300 };

    windowProps.Title = "window1";
    auto window1 = AGI::Window::Create(settings, windowProps);
    auto context1 = AGI::RenderContext::Create(window1);

    windowProps.Title = "window2";
    auto window2 = AGI::Window::Create(settings, windowProps);
    auto context2 = AGI::RenderContext::Create(window2);

    std::thread thread(WindowLoop, context2);
    WindowLoop(context1);

    thread.join();

    delete context1;
    delete context2;
    return 0;
}
