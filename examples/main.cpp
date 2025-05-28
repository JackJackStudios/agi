#include <AGI/agi.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <filesystem>
#include <fstream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

static std::shared_ptr<spdlog::logger> s_ClientLogger;
static std::unique_ptr<AGI::RenderAPI> s_RenderAPI;
static GLFWwindow* s_Window;

static void GLFWErrorCallback(int error, const char* description)
{
    s_ClientLogger->error("GLFW Error ({0}): {1}", error, description);
}

static void OnAGIMessage(std::string_view message, AGI::LogLevel level)
{
    switch (level)
    {
        case AGI::LogLevel::Trace:   s_ClientLogger->trace("{}", std::string(message)); break;
        case AGI::LogLevel::Info:    s_ClientLogger->info("{}", std::string(message)); break;
        case AGI::LogLevel::Warning: s_ClientLogger->warn("{}", std::string(message)); break;
        case AGI::LogLevel::Error:   s_ClientLogger->error("{}", std::string(message)); break;
    }
}

void InitLogging()
{
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");

    s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_ClientLogger);
    s_ClientLogger->set_level(spdlog::level::trace);
    s_ClientLogger->flush_on(spdlog::level::trace);
}

void InitRendering()
{
    glfwInit();
    glfwSetErrorCallback(GLFWErrorCallback);

    s_Window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);

    glfwMakeContextCurrent(s_Window);

    s_RenderAPI = AGI::RenderAPI::Create(
    {
        .PreferedAPI = AGI::APIType::Guess,
        .Blending = true,

        .LoaderFunc = (AGI::OpenGLloaderFn)glfwGetProcAddress,
        .MessageFunc = OnAGIMessage,
    });
}

int main(void)
{
    InitLogging();
    InitRendering();

    std::shared_ptr<AGI::VertexArray> m_SquareVA = AGI::VertexArray::Create();

    float squareVertices[3 * 4] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    std::shared_ptr<AGI::VertexBuffer> squareVB = AGI::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
    squareVB->SetLayout({ { AGI::ShaderDataType::Float3, "a_Position" } });
    m_SquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    std::shared_ptr<AGI::IndexBuffer> squareIB = AGI::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    m_SquareVA->SetIndexBuffer(squareIB);
    
    std::string shaderSrc = R"(
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

    std::shared_ptr<AGI::Shader> m_Shader = AGI::Shader::Create(AGI::Shader::ProcessSource(shaderSrc));

    float aspectRatio = 1280.0f / 720.0f;
    glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f) * glm::mat4(1.0f);

    s_RenderAPI->SetViewport(0, 0, 1280, 720);

    while (!glfwWindowShouldClose(s_Window))
    {
        s_RenderAPI->SetClearColour({ 0.1f, 0.1f, 0.1f, 1 });
        s_RenderAPI->Clear();

        m_Shader->Bind();
        m_Shader->SetMat4("u_ViewProjection", projection);

        s_RenderAPI->DrawIndexed(m_SquareVA);

        glfwSwapBuffers(s_Window);
        glfwPollEvents();
    }

    s_RenderAPI.reset();
    glfwTerminate();
    return 0;
}