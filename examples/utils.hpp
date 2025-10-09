#pragma once

#include <AGI/agi.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

static std::shared_ptr<spdlog::logger> s_ClientLogger;

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
    spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sink->set_pattern("%^[%T] %n: %v%$");
    
    s_ClientLogger = std::make_shared<spdlog::logger>("APP", sink);
    spdlog::register_logger(s_ClientLogger);
    s_ClientLogger->set_level(spdlog::level::trace);
    s_ClientLogger->flush_on(spdlog::level::trace);
}

AGI::Texture LoadTexture(AGI::RenderContext* context, const std::filesystem::path& path)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    AGI_VERIFY(data, "Failed to load sprite \"{}\" ", path.string());

    AGI::TextureSpecification textureSpec;
    textureSpec.Width = width;
    textureSpec.Height = height;
    textureSpec.Format = AGI::Utils::ChannelsToImageFormat(channels);

    AGI::Texture texture = context->CreateTexture(textureSpec);
    texture->SetData(data, width * height * channels);

    return texture;
}
