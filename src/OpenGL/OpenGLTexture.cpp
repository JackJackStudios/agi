#include "agipch.h"
#include "OpenGLTexture.h"

namespace AGI {

    namespace Utils {

        static GLenum GetFormat(TextureSpecifaction spec)
        {
            switch (spec.Format)
            {
                case ImageFormat::RED: return GL_RED;
                case ImageFormat::RG: return GL_RG;
                case ImageFormat::RGB: return GL_RGB;
                case ImageFormat::RGBA: return GL_RGBA;
            }

            AGI_VERIFY(false, "Unsupported OpenGL format");
            return 0;
        }

        static GLenum GetDataType(TextureSpecifaction spec)
        {
            switch (spec.BytesPerChannel)
            {
                case 8:  return GL_UNSIGNED_BYTE;
                case 16: return GL_HALF_FLOAT;
                case 32: return GL_FLOAT;
            }

            AGI_VERIFY(false, "Unsupported OpenGL format");
            return 0;
        }

        static GLenum GetInternalFormat(TextureSpecifaction spec)
        {
            switch (spec.Format)
            {
            case ImageFormat::RED:
                switch (spec.BytesPerChannel)
                {
                case 8:  return GL_R8;
                case 16: return GL_R16F;
                case 32: return GL_R32F;
                }
                break;
            case ImageFormat::RG:
                switch (spec.BytesPerChannel)
                {
                case 8:  return GL_RG8;
                case 16: return GL_RG16F;
                case 32: return GL_RG32F;
                }
                break;
            case ImageFormat::RGB:
                switch (spec.BytesPerChannel)
                {
                case 8:  return GL_RGB8;
                case 16: return GL_RGB16F;
                case 32: return GL_RGB32F;
                }
                break;
            case ImageFormat::RGBA:
                switch (spec.BytesPerChannel)
                {
                case 8:  return GL_RGBA8;
                case 16: return GL_RGBA16F;
                case 32: return GL_RGBA32F;
                }
                break;
            }

            AGI_VERIFY(false, "Unsupported OpenGL format");
            return 0;
        }

        static GLenum GetWrappingType(TextureSpecifaction spec)
        {
            switch (spec.WrappingType)
            {
                case WrappingType::None: return GL_CLAMP_TO_EDGE;
                case WrappingType::ClampBorder: return GL_CLAMP_TO_BORDER;
                case WrappingType::ClampEdge: return GL_CLAMP_TO_EDGE;
                case WrappingType::Repeat: return GL_REPEAT;
                case WrappingType::MirrorRepeat: GL_MIRRORED_REPEAT;
            }

            AGI_VERIFY(false, "Unsupported WrappingType");
            return 0;
        }

    }

    OpenGLTexture::OpenGLTexture(TextureSpecifaction spec)
        : m_Specifaction(spec)
    {
        int channels = ImageFormatToChannels(m_Specifaction.Format);
        int bytesPerPixel = channels * (m_Specifaction.BytesPerChannel / 8);

        GLint alignment = 1;
        if (bytesPerPixel % 8 == 0) alignment = 8;
        else if (bytesPerPixel % 4 == 0) alignment = 4;
        else if (bytesPerPixel % 2 == 0) alignment = 2;

        RenderAPI::GetCurrentAPI()->SetTextureAlignment(alignment);

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
            Utils::GetInternalFormat(m_Specifaction), 
            m_Specifaction.Width, 
            m_Specifaction.Height, 
            0, 
            Utils::GetFormat(m_Specifaction), 
            Utils::GetDataType(m_Specifaction), 
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Specifaction.LinearFiltering ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Specifaction.LinearFiltering ? GL_LINEAR : GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Utils::GetWrappingType(m_Specifaction));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Utils::GetWrappingType(m_Specifaction));

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture::SetData(void* data, uint32_t size)
    {
        AGI_VERIFY(size == m_Specifaction.Width * m_Specifaction.Height * ImageFormatToChannels(m_Specifaction.Format), "Data must be entire texture!");

        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Specifaction.Width, m_Specifaction.Height, Utils::GetFormat(m_Specifaction), GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

}