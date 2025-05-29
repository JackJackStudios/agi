#pragma once

#include <GLFW/glfw3.h>
#include <AGI/agi.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

static std::shared_ptr<spdlog::logger> s_ClientLogger;
static std::unique_ptr<AGI::RenderAPI> s_RenderAPI;

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

GLFWwindow* InitWindow(int width, int height)
{
    glfwInit();
    glfwSetErrorCallback(GLFWErrorCallback);

    GLFWwindow* window = glfwCreateWindow(width, height, EXECUTABLE_NAME, NULL, NULL);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { s_RenderAPI->SetViewport(0, 0, width, height); });
    glfwMakeContextCurrent(window);

    s_RenderAPI = AGI::RenderAPI::Create(
    {
        .PreferedAPI = AGI::APIType::Guess,
        .Blending = true,

        .LoaderFunc = (AGI::OpenGLloaderFn)glfwGetProcAddress,
        .MessageFunc = OnAGIMessage,
    });

    return window;
}

std::shared_ptr<AGI::Texture> LoadTexture(const std::filesystem::path& path)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    AGI_VERIFY(data, "Failed to load sprite \"{}\" ", path.string());

    AGI::TextureSpecification textureSpec;
    textureSpec.Width = width;
    textureSpec.Height = height;
    textureSpec.Format = AGI::ChannelsToImageFormat(channels);

    std::shared_ptr<AGI::Texture> texture = AGI::Texture::Create(textureSpec);
    texture->SetData(data, width * height * channels);

    return texture;
}