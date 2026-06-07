#pragma once

#include <imgui.h>

namespace Horizon {

    inline void SetHorizonDarkThemeColors()
    {
        auto& style = ImGui::GetStyle();
        auto& colors = style.Colors;

        // Custom harmonized dark colors with neon blue accents
        colors[ImGuiCol_WindowBg]             = ImVec4(0.094f, 0.094f, 0.118f, 1.00f); // Very dark slate grey
        colors[ImGuiCol_Header]               = ImVec4(0.165f, 0.165f, 0.204f, 1.00f);
        colors[ImGuiCol_HeaderHovered]        = ImVec4(0.235f, 0.235f, 0.290f, 1.00f);
        colors[ImGuiCol_HeaderActive]         = ImVec4(0.204f, 0.204f, 0.251f, 1.00f);
        
        // Buttons
        colors[ImGuiCol_Button]               = ImVec4(0.141f, 0.141f, 0.176f, 1.00f);
        colors[ImGuiCol_ButtonHovered]        = ImVec4(0.196f, 0.408f, 0.655f, 1.00f); // Soft blue
        colors[ImGuiCol_ButtonActive]         = ImVec4(0.137f, 0.314f, 0.529f, 1.00f); // Deep blue
        
        // Frame Bg
        colors[ImGuiCol_FrameBg]              = ImVec4(0.125f, 0.125f, 0.157f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.165f, 0.165f, 0.204f, 1.00f);
        colors[ImGuiCol_FrameBgActive]        = ImVec4(0.204f, 0.204f, 0.251f, 1.00f);
        
        // Tabs
        colors[ImGuiCol_Tab]                  = ImVec4(0.118f, 0.118f, 0.149f, 1.00f);
        colors[ImGuiCol_TabHovered]           = ImVec4(0.204f, 0.459f, 0.737f, 0.80f);
        colors[ImGuiCol_TabActive]            = ImVec4(0.173f, 0.392f, 0.627f, 1.00f);
        colors[ImGuiCol_TabUnfocused]         = ImVec4(0.098f, 0.098f, 0.125f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive]   = ImVec4(0.141f, 0.141f, 0.176f, 1.00f);
        
        // Title Bar
        colors[ImGuiCol_TitleBg]              = ImVec4(0.078f, 0.078f, 0.098f, 1.00f);
        colors[ImGuiCol_TitleBgActive]        = ImVec4(0.094f, 0.094f, 0.118f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.059f, 0.059f, 0.078f, 1.00f);

        // Resize Grip
        colors[ImGuiCol_ResizeGrip]           = ImVec4(0.196f, 0.408f, 0.655f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.196f, 0.408f, 0.655f, 0.60f);
        colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.196f, 0.408f, 0.655f, 0.90f);

        // Docking
        colors[ImGuiCol_DockingPreview]       = ImVec4(0.196f, 0.408f, 0.655f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg]       = ImVec4(0.078f, 0.078f, 0.098f, 1.00f);

        // Layout rounding & adjustments
        style.WindowRounding = 5.0f;
        style.ChildRounding = 4.0f;
        style.FrameRounding = 4.0f;
        style.PopupRounding = 4.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabRounding = 3.0f;
        style.TabRounding = 4.0f;
        
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 1.0f;
        style.TabBorderSize = 0.0f;
    }

} // namespace Horizon
