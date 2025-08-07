#include "utils.hpp"

static std::string shaderSrc = R"(
    #type vertex
    #version 330 core
			
    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec2 a_TexCoord;

    out vec2 v_TexCoord;

    void main()
    {
	    v_TexCoord = a_TexCoord;
	    gl_Position = vec4(a_Position, 1.0);	
    }

    #type fragment
    #version 330 core
			
    layout(location = 0) out vec4 color;

    in vec2 v_TexCoord;

    uniform sampler2D u_Texture;

    void main()
    {
	    color = texture(u_Texture, v_TexCoord);
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
    settings.Blending = true;

    AGI::WindowProps windowProps;
    windowProps.Title = EXECUTABLE_NAME;
    windowProps.Size = { 720, 720 };

    auto window = AGI::Window::Create(settings, windowProps);
    auto context = AGI::RenderContext::Create(window);

    context->Init();
    
    // Create VAO to hold buffers
    AGI::VertexArray squareVA = context->CreateVertexArray();

    // This is the data that goes into the VBO
    float squareVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    // This is the data that goes into the IBO
    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

    // Create VBO and add to the VAO
    AGI::BufferLayout layout = {
        { AGI::ShaderDataType::Float3, "a_Position" }, 
        { AGI::ShaderDataType::Float2, "a_TexCoord" }
    };

    AGI::VertexBuffer squareVB = context->CreateVertexBuffer(4, layout);
    squareVB->SetData(squareVertices, sizeof(squareVertices));
    squareVA->AddVertexBuffer(squareVB);

    // Create IBO and add to the VAO
    AGI::IndexBuffer squareIB = context->CreateIndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    squareVA->SetIndexBuffer(squareIB);

    // Process shader source, compile and link with uniforms
    AGI::Shader shader = context->CreateShader(AGI::Utils::ProcessSource(shaderSrc));
    shader->SetInt("u_Texture", 0);

    // Load test texture
    AGI::Texture texture = LoadTexture(context, "assets/Checkerboard.png");

    // Main loop now, you know the deal
    while (!window->ShouldClose())
    {
        context->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        context->Clear();

        texture->Bind();
        context->DrawIndexed(squareVA);

        window->OnUpdate();
    }

    context->Shutdown();
    delete context;
    return 0;
}