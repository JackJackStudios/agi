#include "agipch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace AGI {

    OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height, uint16_t channels)
        : m_Width(width), m_Height(height)
    {
        if (channels == 4)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        }

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr);

        SetParams();

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    OpenGLTexture::OpenGLTexture(const std::string& path)
    {
        int width, height, channels;

        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr;
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        AGI_VERIFY(data, "Failed to load image!");

        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        AGI_VERIFY(internalFormat & dataFormat, "Format not supported!");

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

        SetParams();

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture::SetData(void* data, uint32_t size)
    {
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        AGI_VERIFY(size == m_Width * m_Height * bpp, "Data must be entire texture!");

        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
    }

    void OpenGLTexture::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    void OpenGLTexture::SetParams()
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

}