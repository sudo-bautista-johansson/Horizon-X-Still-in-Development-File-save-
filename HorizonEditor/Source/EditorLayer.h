#pragma once

#include "Horizon/Core/Layer.h"
#include "Horizon/Core/Events.h"
#include "Horizon/Renderer/Framebuffer.h"
#include "Horizon/Renderer/OrthographicCamera.h"
#include "EditorCamera.h"
#include "Horizon/Scene/Scene.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/AssetBrowserPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/ScriptEditorPanel.h"
#include "Widgets/Toolbar.h"
#include "Widgets/Gizmos.h"

namespace Horizon {

    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        ~EditorLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;
        
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& e) override;

    private:
        // Startup / Welcome menu state
        bool m_ShowStartup = true;
        int m_StartupTab = 0; // 0=Update Log, 1=Templates, 2=Projects
        std::vector<std::string> m_Templates;
        int m_SelectedTemplate = 0;
        char m_NewProjectName[128] = "";
        std::filesystem::path m_ProjectsRoot;
        std::vector<std::filesystem::path> m_ProjectList;

        void RefreshProjectList();
        bool OnKeyPressed(KeyPressedEvent& e);
        
        void NewScene();
        void OpenScene();
        void SaveSceneAs();

    private:
        Ref<Framebuffer> m_Framebuffer;
        Ref<Scene> m_ActiveScene;

        // Editor camera handling (replaces OrthographicCameraController for editor view)
        EditorCamera m_EditorCamera;

        // Editor Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
        InspectorPanel m_InspectorPanel;
        ConsolePanel m_ConsolePanel;
        AssetBrowserPanel m_AssetBrowserPanel;
        Toolbar m_Toolbar;
        ScriptEditorPanel m_ScriptEditorPanel;

        GizmoType m_GizmoType = GizmoType::Translate;
        std::pair<float, float> m_ViewportSize = { 0.0f, 0.0f };
        // Panel visibility toggles
        bool m_ShowSceneHierarchy = true;
        bool m_ShowInspector = true;
        bool m_ShowConsole = true;
        bool m_ShowAssetBrowser = true;
        bool m_ShowViewport = true;
        bool m_ShowProfiler = false;
        bool m_ShowTerminal = false;
        ViewportPanel m_ViewportPanel;
        // Runtime scene for play mode
        Ref<Scene> m_RuntimeScene;
        bool m_IsPlaying = false;
    };

} // namespace Horizon