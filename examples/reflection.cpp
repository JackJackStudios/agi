#include "utils.hpp"

static std::string shaderSrc = R"(
    #type vertex
    #version 330 core
			
    layout(location = 0) in vec3 a_Position;

	out vec3 v_Position;

    void main()
    {
		v_Position = a_Position;
	    gl_Position = vec4(a_Position, 1.0);	
    }

    #type fragment
    #version 330 core
			
    layout(location = 0) out vec4 color;

	in vec3 v_Position;

    void main()
    {
	    color = vec4(v_Position * 0.5 + 0.5, 1.0);
    }
)";

int main(void)
{
    // Init spdlog for AGI callbacks
    InitLogging();

    // Create GLFW window and the AGI::RenderContext
    AGI::Settings settings;
    settings.PreferedAPI = AGI::APIType::Guess;
    settings.MessageFunc = OnAGIMessage;
    settings.Blending = true;

    settings.Window.Width = 1280;
    settings.Window.Height = 720;
    settings.Window.Title = EXECUTABLE_NAME;

    auto window = AGI::Window::Create(settings);
    auto context = AGI::RenderContext::Create(window);

    context->Init();

    // Process shader source, compile and link
    AGI::Shader shader = context->CreateShader(AGI::ProcessSource(shaderSrc));
    std::vector<AGI::Attribute> attributes = AGI::Reflection::GetAttributes(shader);

    for (const auto& attr : attributes)
    {
        std::cout << "Attribute" << " name=" << attr.Name
            << ", size=" << attr.Size
            << ", location=" << attr.Location << "\n";
    }

    // Main loop now, you know the deal
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