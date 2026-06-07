#include "AssetBrowserPanel.h"
#include <imgui.h>
#include <iostream>
#include "../Scripting/ScriptingSystem.h"
#include "../Systems/ScriptManager.h"
#include "Horizon/Core/Log.h"

namespace Horizon {

    // Default to the engine's asset path or workspace directory
    AssetBrowserPanel::AssetBrowserPanel()
    {
        // Check if Sandbox/Assets exists, otherwise use current path
        std::filesystem::path sandboxAssets = "Sandbox/Assets";
        if (std::filesystem::exists(sandboxAssets))
            m_CurrentDirectory = sandboxAssets;
        else
            m_CurrentDirectory = std::filesystem::current_path();

    // Register ScriptManager open callback to refresh asset browser on create
    ScriptManager::SetOpenCallback([this](const std::filesystem::path& p) {
        // If created inside this directory, refresh or jump to file
        if (p.parent_path() == m_CurrentDirectory)
        {
            // Use core log macro
            HZ_CORE_INFO("AssetBrowser: detected new script {0}", p.string());
        }
    });
    }

    void AssetBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Asset Browser");

        // Breadcrumb Navigation
        if (m_CurrentDirectory != std::filesystem::path("Sandbox/Assets") && m_CurrentDirectory != std::filesystem::current_path())
        {
            if (ImGui::Button("<- Back"))
            {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
            ImGui::SameLine();
        }

        ImGui::Text("Path: %s", m_CurrentDirectory.string().c_str());
        ImGui::Separator();

        // Grid parameters
        float padding = 16.0f;
        float thumbnailSize = 80.0f;
        float cellSize = thumbnailSize + padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;

        ImGui::Columns(columnCount, 0, false);

        std::error_code ec;
        for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory, ec))
        {
            const auto& path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, m_CurrentDirectory);
            std::string filenameString = relativePath.filename().string();

            // Set button color based on whether it is a directory or file
            bool isDir = directoryEntry.is_directory();
            ImGui::PushID(filenameString.c_str());

            if (isDir)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.3f, 0.4f, 1.0f));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
            }

            // Simple button representing the folder/file thumbnail
            ImVec2 btnSize = ImVec2(thumbnailSize, thumbnailSize);
            if (ImGui::Button(isDir ? "[Dir]" : "[File]", btnSize))
            {
                if (isDir)
                {
                    m_CurrentDirectory /= path.filename();
                }
            }

            ImGui::PopStyleColor();

            // Single click info or drag drops can go here
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if (!isDir)
                {
                    // If file is a .lua script, open via ScriptManager callback
                    if (path.extension() == ".lua")
                    {
                        // Open existing script directly in editor
                        ScriptManager::SetOpenCallback([this](const std::filesystem::path& p) { HZ_CORE_INFO("Script open callback: {0}", p.string()); });
                        ScriptingSystem::Get().OpenScriptInEditor(path);
                    }
                }
            }

            ImGui::TextWrapped("%s", filenameString.c_str());

            ImGui::NextColumn();
            ImGui::PopID();
        }

        ImGui::Columns(1);

        ImGui::End();
    }

} // namespace Horizon
