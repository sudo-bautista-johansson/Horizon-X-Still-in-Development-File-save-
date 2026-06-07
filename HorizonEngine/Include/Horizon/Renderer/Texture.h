#pragma once

#include "Horizon/Core/Base.h"
#include <string>

namespace Horizon {

    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetRendererID() const = 0;

        virtual void SetData(void* data, uint32_t size) = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;

        virtual bool IsLoaded() const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        Texture2D(uint32_t width, uint32_t height);
        Texture2D(const std::string& path);
        ~Texture2D() override;

        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }
        uint32_t GetRendererID() const override { return m_RendererID; }

        void SetData(void* data, uint32_t size) override;

        void Bind(uint32_t slot = 0) const override;

        bool IsLoaded() const override { return m_IsLoaded; }

        static Ref<Texture2D> Create(uint32_t width, uint32_t height);
        static Ref<Texture2D> Create(const std::string& path);
        const std::string& GetPath() const;

    private:
        std::string m_Path;
        bool m_IsLoaded = false;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        unsigned int m_InternalFormat, m_DataFormat;
    };

} // namespace Horizon
