#pragma once

#include "Horizon/Core/Base.h"
#include "Horizon/Renderer/Framebuffer.h"

#include <glm/glm.hpp>
#include <utility>
#include <./templates/templates.h>

namespace Horizon
{
    class ViewportPanel
    {
    public:

        ViewportPanel();

        std::pair<float, float> OnImGuiRender(const Ref<Framebuffer>& framebuffer);

        // State
        bool IsHovered() const;
        bool IsFocused() const;

        // Viewport info
        const glm::vec2& GetSize() const;
        const glm::vec2* GetBounds() const;

        // Convert absolute screen coordinates to viewport-local coordinates (pixels)
        glm::vec2 ScreenToViewport(const glm::vec2& screenPos) const;

        // Picking stub: returns entity id or -1 if none (placeholder)
        int PickEntityAt(const glm::vec2& viewportCoords) const;

    private:

        Ref<Framebuffer> m_Framebuffer;
        // Last picked entity id from most recent pick
        int m_LastPickedID = -1;

        void UpdateViewportBounds();

    private:

        bool m_ViewportHovered = false;
        bool m_ViewportFocused = false;

        glm::vec2 m_ViewportSize = { 1280.0f, 720.0f };

        // [0] = Min
        // [1] = Max
        glm::vec2 m_ViewportBounds[2];
    };
}