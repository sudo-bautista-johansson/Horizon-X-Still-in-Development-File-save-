#include "InspectorPanel.h"
#include "Horizon/ECS/Components.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Horizon/Renderer/GLM.h"
#include "Horizon/Renderer/Texture.h"

namespace Horizon {

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGuiIO& io = ImGui::GetIO();
    ImFont* boldFont = io.Fonts->Fonts.size() > 0 ? io.Fonts->Fonts[0] : nullptr; // fallback

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        // X
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Y
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Z
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    void InspectorPanel::OnImGuiRender(Entity selectedEntity)
    {
        ImGui::Begin("Inspector");

        if (selectedEntity)
        {
            DrawComponents(selectedEntity);
        }

        ImGui::End();
    }

    void InspectorPanel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strncpy(buffer, tag.c_str(), sizeof(buffer));
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }

            ImGui::SameLine();
            ImGui::PushItemWidth(-1);

            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponent");

            if (ImGui::BeginPopup("AddComponent"))
            {
                if (ImGui::MenuItem("Sprite Renderer"))
                {
                    if (!entity.HasComponent<SpriteRendererComponent>())
                        entity.AddComponent<SpriteRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::MenuItem("Camera"))
                {
                    if (!entity.HasComponent<CameraComponent>())
                        entity.AddComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::PopItemWidth();
        }

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

        if (entity.HasComponent<TransformComponent>())
        {
            bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
            if (open)
            {
                auto& tc = entity.GetComponent<TransformComponent>();
                DrawVec3Control("Translation", tc.Translation);
                DrawVec3Control("Rotation", tc.Rotation);
                DrawVec3Control("Scale", tc.Scale, 1.0f);

                ImGui::TreePop();
            }
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");
            ImGui::PopStyleVar();

            if (open)
            {
                auto& src = entity.GetComponent<SpriteRendererComponent>();
                ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
                
                // Add dummy select box for texture loading
                if (ImGui::Button("Load Texture", ImVec2(100, 0)))
                {
                    // Fallback to load default grid
                    src.Texture = Texture2D::Create("Assets/fallback.png");
                }
                ImGui::SameLine();
                if (src.Texture)
                {
                    ImGui::Text("Texture: Yes (ID: %d)", src.Texture->GetRendererID());
                    if (ImGui::Button("Clear Texture"))
                        src.Texture = nullptr;
                }
                else
                {
                    ImGui::Text("Texture: None");
                }

                ImGui::TreePop();
            }
        }

        if (entity.HasComponent<CameraComponent>())
        {
            bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera");
            if (open)
            {
                auto& cc = entity.GetComponent<CameraComponent>();
                ImGui::Checkbox("Primary", &cc.Primary);
                ImGui::DragFloat("Size", &cc.OrthoSize, 0.1f, 0.1f, 100.0f);
                ImGui::DragFloat("Near", &cc.OrthoNear, 0.1f, -100.0f, 100.0f);
                ImGui::DragFloat("Far", &cc.OrthoFar, 0.1f, -100.0f, 100.0f);

                ImGui::TreePop();
            }
        }
    }

} // namespace Horizon
