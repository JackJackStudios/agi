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

    // Create VAO to hold buffers
    AGI::VertexArray squareVA = context->CreateVertexArray();

    // This is the data that goes into the VBO
    float squareVertices[3 * 4] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    // This is the data that goes into the IBO
    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

    // Create VBO and add to the VAO
    AGI::BufferLayout layout = {
        { AGI::ShaderDataType::Float3, "a_Position" }
    };

    AGI::VertexBuffer squareVB = context->CreateVertexBuffer(4, layout);
    squareVB->SetData(squareVertices, sizeof(squareVertices));
    squareVA->AddVertexBuffer(squareVB);

    // Create IBO and add to the VAO
    AGI::IndexBuffer squareIB = context->CreateIndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    squareVA->SetIndexBuffer(squareIB);

    // Process shader source, compile and link
    AGI::Shader shader = context->CreateShader(AGI::Utils::ProcessSource(shaderSrc));

    // Main loop now, you know the deal
    while (!window->ShouldClose())
    {
        context->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        context->Clear();
        
        context->DrawIndexed(squareVA);

        window->OnUpdate();
    }

    context->Shutdown();
    window.reset();
    return 0;
}