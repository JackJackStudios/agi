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

    // Create GLFW window and the AGI::RenderAPI
    GLFWwindow* window = InitWindow(720, 720);

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
    std::shared_ptr<AGI::Shader> shader = AGI::Shader::Create(AGI::Shader::ProcessSource(shaderSrc));
    shader->SetInt("u_Texture", 0);

    // Load test texture
    std::shared_ptr<AGI::Texture> texture = LoadTexture("assets/Checkerboard.png");

    // Main loop now, you know the deal
    while (!glfwWindowShouldClose(window))
    {
        s_RenderAPI->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        s_RenderAPI->Clear();

        texture->Bind();
        s_RenderAPI->DrawIndexed(squareVA);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    s_RenderAPI.reset();
    glfwTerminate();
    return 0;
}