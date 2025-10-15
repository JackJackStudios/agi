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
    settings.PreferedAPI = AGI::BestAPI();
    settings.MessageFunc = OnAGIMessage;

    AGI::WindowProps windowProps;
    windowProps.Title = EXECUTABLE_NAME;

    auto window = AGI::Window::Create(settings, windowProps);
    auto context = AGI::RenderContext::Create(window);

    context->Init();

    // Process shader source, compile and link
    AGI::Shader shader = context->CreateShader(AGI::Utils::ProcessSource(shaderSrc));
    AGI::BufferLayout layout = shader->GetLayout();

    for (int i=0; i<layout.GetSize(); i++)
        AGI_INFO("Attribute #{} \"{}\" (Size: {}) ", i, layout[i].Name, layout[i].Size);

    context->Shutdown();
    delete context;
    
    std::cin.get();
    return 0;
}