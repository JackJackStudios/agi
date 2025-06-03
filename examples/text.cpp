#include "utils.hpp"

static std::string vertexSrc = R"(
    #version 330 core
			
	layout(location = 0) in vec3 a_Position;
	layout(location = 1) in vec2 a_TexCoord;

	out vec2 v_TexCoord;

	void main()
	{
		v_TexCoord = a_TexCoord;
		gl_Position = vec4(a_Position, 1.0);	
	}
)";

int main(void)
{
    // Init spdlog for AGI callbacks
    InitLogging();

    // Create GLFW window and the AGI::RenderAPI
    AGI::Settings settings;
    settings.PreferedAPI = AGI::APIType::Guess;
    settings.MessageFunc = OnAGIMessage;
    settings.Blending = true;

    settings.WindowProps.Width = 720;
    settings.WindowProps.Height = 720;
    settings.WindowProps.Title = EXECUTABLE_NAME;

    auto renderAPI = AGI::RenderContext::Create(settings);
    auto window = AGI::Window::Create(renderAPI, true);

    // Create VAO to hold buffers
    std::shared_ptr<AGI::VertexArray> squareVA = AGI::VertexArray::Create();

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
    std::shared_ptr<AGI::VertexBuffer> squareVB = AGI::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
    squareVB->SetLayout({
        { AGI::ShaderDataType::Float3, "a_Position" },
        { AGI::ShaderDataType::Float2, "a_TexCoord" }
        });
    squareVA->AddVertexBuffer(squareVB);

    // Create IBO and add to the VAO
    std::shared_ptr<AGI::IndexBuffer> squareIB = AGI::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    squareVA->SetIndexBuffer(squareIB);

    // Process shader source, compile and link with uniforms
    AGI::ShaderSources src;
    src[AGI::ShaderType::Vertex] = vertexSrc;
    src[AGI::ShaderType::Fragment] = AGI::Font::GetDefaultShader();
    
    std::shared_ptr<AGI::Shader> shader = AGI::Shader::Create(src);
    shader->SetInt("u_Texture", 0);

    // Load test texture
    std::shared_ptr<AGI::Font> font = std::make_shared<AGI::Font>("C:/Windows/Fonts/arial.ttf");

    std::shared_ptr<AGI::Texture> texture = font->GetAtlasTexture();

    // Main loop now, you know the deal
    while (!window->ShouldClose())
    {
        renderAPI->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        renderAPI->Clear();

        texture->Bind();
        renderAPI->DrawIndexed(squareVA);

        window->OnUpdate();
    }

    renderAPI.reset();
    window.reset();
    return 0;
}