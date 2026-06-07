#include "ViewportPanel.h"

#include <imgui.h>

namespace Horizon
{
    ViewportPanel::ViewportPanel()
    {
    }

    std::pair<float, float> ViewportPanel::OnImGuiRender(const Ref<Framebuffer>& framebuffer)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("Viewport");

        // ---------------------------------------------------------
        // Viewport State
        // ---------------------------------------------------------

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        // ---------------------------------------------------------
        // Viewport Bounds
        // ---------------------------------------------------------

        UpdateViewportBounds();

        // ---------------------------------------------------------
        // Viewport Size
        // ---------------------------------------------------------

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        m_ViewportSize = {
            viewportPanelSize.x,
            viewportPanelSize.y
        };

        // ---------------------------------------------------------
        // Framebuffer Rendering
        // ---------------------------------------------------------

        // store the framebuffer reference for picking
        m_Framebuffer = framebuffer;

        if (framebuffer)
        {
            uint64_t textureID =
                framebuffer->GetColorAttachmentRendererID();

            ImGui::Image(
                reinterpret_cast<void*>(textureID),
                ImVec2(m_ViewportSize.x, m_ViewportSize.y),
                ImVec2(0.0f, 1.0f),
                ImVec2(1.0f, 0.0f)
            );
        }

        ImGui::End();

        ImGui::PopStyleVar();

        return { m_ViewportSize.x, m_ViewportSize.y }; // no-op patch to mark file update
    }

    void ViewportPanel::UpdateViewportBounds()
    {
        ImVec2 minRegion = ImGui::GetWindowContentRegionMin();
        ImVec2 maxRegion = ImGui::GetWindowContentRegionMax();
        ImVec2 windowPos = ImGui::GetWindowPos();

        m_ViewportBounds[0] = {
            minRegion.x + windowPos.x,
            minRegion.y + windowPos.y
        };

        m_ViewportBounds[1] = {
            maxRegion.x + windowPos.x,
            maxRegion.y + windowPos.y
        };
    }

    bool ViewportPanel::IsHovered() const
    {
        return m_ViewportHovered;
    }

    bool ViewportPanel::IsFocused() const
    {
        return m_ViewportFocused;
    }

    const glm::vec2& ViewportPanel::GetSize() const
    {
        return m_ViewportSize;
    }

    const glm::vec2* ViewportPanel::GetBounds() const
    {
        return m_ViewportBounds;
    }

    glm::vec2 ViewportPanel::ScreenToViewport(const glm::vec2& screenPos) const
    {
        // screenPos is absolute (window) coords; convert to local viewport coords
        glm::vec2 min = m_ViewportBounds[0];
        return glm::vec2(screenPos.x - min.x, screenPos.y - min.y);
    }

    int ViewportPanel::PickEntityAt(const glm::vec2& viewportCoords) const
    {
        // Convert to framebuffer coordinates (origin bottom-left)
        int x = (int)viewportCoords.x;
        int y = (int)(m_ViewportSize.y - viewportCoords.y);
        if (!m_Framebuffer)
            return -1;
        // Read from ID attachment (attachment index 1)
        int id = m_Framebuffer->ReadPixel(1, x, y);
        return id;
    }
}