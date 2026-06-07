#pragma once

#include "Horizon/Core/Base.h"

namespace Horizon {

    struct FramebufferSpecification
    {
        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t Samples = 1;
        bool SwapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        Framebuffer(const FramebufferSpecification& spec);
        ~Framebuffer();

        void Bind();
        void Unbind();

        void Resize(uint32_t width, uint32_t height);

        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
        const FramebufferSpecification& GetSpecification() const { return m_Specification; }

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

        // Read integer pixel from ID attachment at x,y
        int ReadPixel(uint32_t attachmentIndex, int x, int y) const;

    private:
        void Invalidate();

    private:
        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment = 0;
        uint32_t m_DepthAttachment = 0;
        // Optional ID attachment for entity picking (integer attachment)
        uint32_t m_IDAttachment = 0;
        FramebufferSpecification m_Specification;
    };

} // namespace Horizon
