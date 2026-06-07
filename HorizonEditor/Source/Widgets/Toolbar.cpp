#include "Toolbar.h"
#include <imgui.h>

namespace Horizon {

    Toolbar::Toolbar()
    {
    }

    void Toolbar::OnImGuiRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

        ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        float size = ImGui::GetWindowHeight() - 4.0f;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 1.5f));

        // Play Button
        bool isPlay = m_State == SceneState::Play;
        if (isPlay)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
        if (ImGui::Button(isPlay ? "[Playing]" : "Play", ImVec2(size * 1.2f, size)))
        {
            m_State = SceneState::Play;
        }
        if (isPlay)
            ImGui::PopStyleColor();

        ImGui::SameLine();

        // Pause Button
        bool isPause = m_State == SceneState::Pause;
        if (isPause)
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.8f, 0.2f, 1.0f));
        if (ImGui::Button("Pause", ImVec2(size * 1.2f, size)))
        {
            if (m_State == SceneState::Play)
                m_State = SceneState::Pause;
            else if (m_State == SceneState::Pause)
                m_State = SceneState::Play;
        }
        if (isPause)
            ImGui::PopStyleColor();

        ImGui::SameLine();

        // Stop Button
        if (ImGui::Button("Stop", ImVec2(size * 1.2f, size)))
        {
            m_State = SceneState::Edit;
        }

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(2);
        ImGui::End();
    }

} // namespace Horizon