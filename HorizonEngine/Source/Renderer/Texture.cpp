#include "Horizon/Renderer/Texture.h"
#include "Horizon/Core/Log.h"
#include <glad/glad.h>

#include "Horizon/Renderer/stb_image.h"

namespace Horizon {

    const std::string& Texture2D::GetPath() const { return m_Path; }

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        return CreateRef<Texture2D>(width, height);
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        return CreateRef<Texture2D>(path);
    }

    Texture2D::Texture2D(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height), m_Path("")
    {
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    Texture2D::Texture2D(const std::string& path)
        : m_Path(path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
        if (data)
        {
            m_Width = (uint32_t)width; m_Height = (uint32_t)height;
            m_InternalFormat = GL_RGBA8; m_DataFormat = GL_RGBA;

            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            m_IsLoaded = true;
        }
        else
        {
            // fallback
            m_Width = 128; m_Height = 128; m_InternalFormat = GL_RGBA8; m_DataFormat = GL_RGBA;
            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
            uint32_t* pixels = new uint32_t[m_Width * m_Height];
            for (uint32_t i = 0; i < m_Width * m_Height; ++i) pixels[i] = 0xFF00FF00; // green fallback
            glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, pixels);
            delete[] pixels;
            m_IsLoaded = false;
        }
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture2D::SetData(void* data, uint32_t size)
    {
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        HZ_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void Texture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }

} // namespace Horizon
