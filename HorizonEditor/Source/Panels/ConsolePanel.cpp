#include "ConsolePanel.h"
#include "Horizon/Core/Log.h"
#include <imgui.h>

namespace Horizon {

    ConsolePanel::ConsolePanel()
    {
    }

    void ConsolePanel::OnImGuiRender()
    {
        ImGui::Begin("Console");

        // ── Top Controls Bar ──────────────────────────────────────────────
        if (ImGui::Button("Clear"))
        {
            Log::ClearMessages();
        }
        ImGui::SameLine();
        ImGui::Checkbox("Auto-Scroll", &m_AutoScroll);
        
        ImGui::SameLine();
        // Vertical separator fallback
        ImGui::Dummy(ImVec2(10, 0));
        ImGui::SameLine();

        ImGui::Checkbox("Info", &m_ShowInfo);
        ImGui::SameLine();
        ImGui::Checkbox("Warnings", &m_ShowWarn);
        ImGui::SameLine();
        ImGui::Checkbox("Errors", &m_ShowError);

        ImGui::SameLine();
        ImGui::Text("  Search:");
        ImGui::SameLine();
        ImGui::InputText("##ConsoleSearch", m_SearchFilter, sizeof(m_SearchFilter));

        ImGui::Separator();

        // ── Log Messages Window ───────────────────────────────────────────
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        std::lock_guard<std::mutex> lock(Log::GetMutex());
        const auto& messages = Log::GetMessages();

        std::string searchStr(m_SearchFilter);
        for (const auto& msg : messages)
        {
            // Apply filtering logic
            if (msg.Level == LogLevel::Info && !m_ShowInfo) continue;
            if (msg.Level == LogLevel::Warn && !m_ShowWarn) continue;
            if (msg.Level == LogLevel::Error && !m_ShowError) continue;

            if (!searchStr.empty())
            {
                if (msg.Message.find(searchStr) == std::string::npos)
                    continue;
            }

            // Set color based on log level
            ImVec4 color = ImVec4(0.85f, 0.85f, 0.85f, 1.0f); // Default white/grey
            switch (msg.Level)
            {
                case LogLevel::Trace: color = ImVec4(0.55f, 0.55f, 0.55f, 1.0f); break; // Grey
                case LogLevel::Info:  color = ImVec4(0.2f, 0.85f, 0.2f, 1.0f);   break; // Green
                case LogLevel::Warn:  color = ImVec4(0.95f, 0.8f, 0.1f, 1.0f);   break; // Yellow
                case LogLevel::Error: color = ImVec4(0.9f, 0.15f, 0.15f, 1.0f);  break; // Red
            }

            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "[%s]", msg.Time.c_str());
            ImGui::SameLine();
            ImGui::TextColored(color, "%s", msg.Message.c_str());
        }

        if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();

        ImGui::End();
    }

} // namespace Horizon
