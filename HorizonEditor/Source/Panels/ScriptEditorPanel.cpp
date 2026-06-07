#include "ScriptEditorPanel.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <fstream>
#include <sstream>

#include "Horizon/Core/Log.h"
#include "../Scripting/ScriptingSystem.h"

namespace Horizon
{
    ScriptEditorPanel::ScriptEditorPanel()
    {
    }

    void ScriptEditorPanel::OpenFile(const std::filesystem::path& path)
    {
        m_OpenPath = path;
        m_IsOpen = true;

        std::ifstream ifs(path);

        if (!ifs.is_open())
        {
            HZ_CORE_ERROR("Failed to open script: {0}", path.string());
            return;
        }

        std::stringstream ss;
        ss << ifs.rdbuf();

        m_Buffer = ss.str();

        m_Dirty = false;

        HZ_CORE_INFO("Opened script: {0}", path.string());
    }

    void ScriptEditorPanel::OnImGuiRender()
    {
        if (!m_IsOpen)
            return;

        AutoSave();

        ImGui::Begin("Script Editor", &m_IsOpen);

        // ---------------------------------------------------------
        // Toolbar
        // ---------------------------------------------------------

        if (ImGui::Button("Save"))
        {
            Save();
        }

        ImGui::SameLine();

        if (ImGui::Button("Reload"))
        {
            Reload();
        }

        ImGui::SameLine();

        if (ImGui::Button("Open in VSCode"))
        {
            std::string cmd =
                "code \"" + m_OpenPath.string() + "\"";

            system(cmd.c_str());
        }

        ImGui::SameLine();

        if (ImGui::Button("Copy"))
        {
            CopyToClipboard();
        }

        ImGui::SameLine();

        if (ImGui::Button("Paste"))
        {
            PasteFromClipboard();
        }

        ImGui::Separator();

        // ---------------------------------------------------------
        // File Info
        // ---------------------------------------------------------

        ImGui::Text("File: %s", m_OpenPath.string().c_str());

        if (m_Dirty)
        {
            ImGui::SameLine();
            ImGui::TextColored(
                ImVec4(1.0f, 0.8f, 0.2f, 1.0f),
                "* Unsaved"
            );
        }

        ImGui::Separator();

        // ---------------------------------------------------------
        // Script Editor
        // ---------------------------------------------------------

        ImVec2 editorSize = ImGui::GetContentRegionAvail();

        bool modified = ImGui::InputTextMultiline(
            "##ScriptEditor",
            &m_Buffer,
            editorSize,
            ImGuiInputTextFlags_AllowTabInput
        );

        if (modified)
        {
            MarkDirty();
        }

        ImGui::End();
    }

    void ScriptEditorPanel::Save()
    {
        if (m_OpenPath.empty())
            return;

        std::ofstream ofs(m_OpenPath);

        if (!ofs.is_open())
        {
            HZ_CORE_ERROR(
                "Failed to save script: {0}",
                m_OpenPath.string()
            );

            return;
        }

        ofs << m_Buffer;

        ofs.close();

        m_Dirty = false;

        HZ_CORE_INFO(
            "Saved script: {0}",
            m_OpenPath.string()
        );

        ScriptingSystem::Get().ReloadScripts();
    }

    void ScriptEditorPanel::AutoSave()
    {
        if (!m_IsOpen || !m_Dirty)
            return;

        double currentTime = ImGui::GetTime();

        if (currentTime - m_LastAutoSaveTime < 60.0)
            return;

        std::filesystem::path autoSavePath =
            m_OpenPath;

        std::ofstream ofs(autoSavePath);

        if (!ofs.is_open())
        {
            HZ_CORE_ERROR(
                "Failed to auto-save script: {0}",
                autoSavePath.string()
            );

            return;
        }

        ofs << m_Buffer;

        ofs.close();

        m_LastAutoSaveTime = currentTime;

        HZ_CORE_INFO(
            "Auto-saved script: {0}",
            autoSavePath.string()
        );
    }

    void ScriptEditorPanel::MarkDirty()
    {
        m_Dirty = true;
    }

    void ScriptEditorPanel::Close()
    {
        m_IsOpen = false;

        m_Buffer.clear();

        m_OpenPath.clear();

        m_Dirty = false;
    }

    void ScriptEditorPanel::Reload()
    {
        if (m_OpenPath.empty())
            return;

        OpenFile(m_OpenPath);

        HZ_CORE_INFO(
            "Reloaded script: {0}",
            m_OpenPath.string()
        );
    }

    void ScriptEditorPanel::NewFile()
    {
        m_Buffer.clear();

        m_OpenPath.clear();

        m_Dirty = false;

        m_IsOpen = true;
    }

    void ScriptEditorPanel::Rename(
        const std::filesystem::path& newPath
    )
    {
        if (m_OpenPath.empty())
            return;

        if (!std::filesystem::exists(m_OpenPath))
        {
            HZ_CORE_ERROR(
                "Cannot rename missing file: {0}",
                m_OpenPath.string()
            );

            return;
        }

        std::filesystem::rename(
            m_OpenPath,
            newPath
        );

        HZ_CORE_INFO(
            "Renamed script: {0} -> {1}",
            m_OpenPath.string(),
            newPath.string()
        );

        m_OpenPath = newPath;
    }

    void ScriptEditorPanel::Delete()
    {
        if (m_OpenPath.empty())
            return;

        if (!std::filesystem::exists(m_OpenPath))
        {
            HZ_CORE_ERROR(
                "Cannot delete missing file: {0}",
                m_OpenPath.string()
            );

            return;
        }

        std::filesystem::remove(m_OpenPath);

        HZ_CORE_INFO(
            "Deleted script: {0}",
            m_OpenPath.string()
        );

        Close();
    }

    void ScriptEditorPanel::Duplicate()
    {
        if (m_OpenPath.empty())
            return;

        std::filesystem::path duplicatePath =
            m_OpenPath.parent_path() /
            (m_OpenPath.stem().string() +
                "_copy" +
                m_OpenPath.extension().string());

        std::filesystem::copy_file(
            m_OpenPath,
            duplicatePath,
            std::filesystem::copy_options::overwrite_existing
        );

        HZ_CORE_INFO(
            "Duplicated script: {0}",
            duplicatePath.string()
        );

        OpenFile(duplicatePath);
    }

    void ScriptEditorPanel::Move(
        const std::filesystem::path& newDirectory
    )
    {
        if (m_OpenPath.empty())
            return;

        std::filesystem::path newPath =
            newDirectory /
            m_OpenPath.filename();

        std::filesystem::rename(
            m_OpenPath,
            newPath
        );

        HZ_CORE_INFO(
            "Moved script: {0}",
            newPath.string()
        );

        m_OpenPath = newPath;
    }

    void ScriptEditorPanel::CopyToClipboard()
    {
        ImGui::SetClipboardText(
            m_Buffer.c_str()
        );

        HZ_CORE_INFO(
            "Copied script to clipboard"
        );
    }

    void ScriptEditorPanel::PasteFromClipboard()
    {
        const char* clipboard =
            ImGui::GetClipboardText();

        if (!clipboard)
        {
            HZ_CORE_ERROR(
                "Clipboard is empty"
            );

            return;
        }

        m_Buffer += clipboard;

        MarkDirty();

        HZ_CORE_INFO(
            "Pasted from clipboard"
        );
    }
}