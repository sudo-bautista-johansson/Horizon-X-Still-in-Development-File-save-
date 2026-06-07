#include "Gizmos.h"
#include "Horizon/ECS/Components.h"
#include <imgui.h>

namespace Horizon {

    void Gizmos::Draw(Entity selectedEntity, const OrthographicCamera& camera, GizmoType type)
    {
        if (!selectedEntity || !selectedEntity.HasComponent<TransformComponent>())
            return;

        // Render a overlay controls window inside the viewport panel
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + 10, ImGui::GetWindowPos().y + 30));
        ImGui::BeginChild("GizmoControls", ImVec2(180, 95), true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

        auto& tc = selectedEntity.GetComponent<TransformComponent>();
        
        switch (type)
        {
            case GizmoType::Translate:
            {
                ImGui::Text("Gizmo: Translate");
                ImGui::DragFloat2("Pos", &tc.Translation.x, 0.05f);
                break;
            }
            case GizmoType::Rotate:
            {
                ImGui::Text("Gizmo: Rotate");
                ImGui::DragFloat("Rot Z", &tc.Rotation.z, 0.5f);
                break;
            }
            case GizmoType::Scale:
            {
                ImGui::Text("Gizmo: Scale");
                ImGui::DragFloat2("Scale", &tc.Scale.x, 0.05f);
                break;
            }
        }

        ImGui::Text("Keys: W(T) E(R) R(S)");
        ImGui::EndChild();
    }

} // namespace Horizon
