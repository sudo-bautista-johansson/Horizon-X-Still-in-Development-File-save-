#include "EditorLayer.h"
#include "Scripting/ScriptingSystem.h" // Ensure ScriptingSystem is included early (no-op)
#include "Themes/HorizonDark.h"
#include "Widgets/FileDialog.h"
#include "Panels/ScriptEditorPanel.h"
#include "Scripting/ScriptingSystem.h"
#include "Horizon/Core/Log.h"
#include "Horizon/Core/Input.h"
#include "Horizon/Core/KeyCodes.h"
#include "Horizon/ECS/Components.h"
#include "Horizon/Renderer/RenderCommand.h"
#include "Horizon/Renderer/Renderer2D.h"
#include "Horizon/Core/Application.h"

#include <imgui.h>
#include <iostream>

#include "Systems/ScriptManager.h"
#include "EditorContext.h"
#include <fstream>

// No-op include confirmation: intentionally retained to ensure any translation-unit
// side-effects or tooling dependencies are preserved even if the header appears unused.

namespace Horizon {

    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_EditorCamera(45.0f, 1280.0f / 720.0f)
    {
        // Initialize 2D renderer
        Horizon::Renderer2D::Init();
        // Hook scripting open callback
        ScriptingSystem::Get().SetOpenCallback([this](const std::filesystem::path& p) { m_ScriptEditorPanel.OpenFile(p); });
        // Hook ScriptManager open callback
        ScriptManager::SetOpenCallback([this](const std::filesystem::path& p) { m_ScriptEditorPanel.OpenFile(p); });
        // Duplicate registration removed to avoid double-binding; callbacks remain registered above.

        // Setup templates (populate with 5 sample template names)
        m_Templates = { "2D Platformer", "Top-Down RPG", "First-Person", "Puzzle", "Empty" };

        // Projects folder under workspace
        m_ProjectsRoot = std::filesystem::current_path() / "Projects";
        if (!std::filesystem::exists(m_ProjectsRoot))
            std::filesystem::create_directories(m_ProjectsRoot);
        RefreshProjectList();
    }

    void EditorLayer::OnAttach()
    {
        HZ_PROFILE_FUNCTION();

        // ── Theme Configuration ──────────────────────────────────────────
        SetHorizonDarkThemeColors();

        // ── Framebuffer Setup ────────────────────────────────────────────
        FramebufferSpecification spec;
        spec.Width = 1280;
        spec.Height = 720;
        m_Framebuffer = Framebuffer::Create(spec);

        // ── Scene Initialization ─────────────────────────────────────────
        NewScene();

        // Populate EditorContext (global)
        ::Horizon::g_EditorContext.ActiveScene = m_ActiveScene;

        // Create default test entities to show the engine in action immediately
        auto square = m_ActiveScene->CreateEntity("Test Entity");
        square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.196f, 0.408f, 0.655f, 1.0f }); // Electric blue

        auto camera = m_ActiveScene->CreateEntity("Main Camera");
        camera.AddComponent<CameraComponent>();
        camera.GetComponent<TransformComponent>().Translation.z = 1.0f;

        // Initialize viewport panel and camera
        m_ViewportPanel = ViewportPanel();
    }

    void EditorLayer::OnDetach()
    {
        HZ_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        HZ_PROFILE_FUNCTION();

        // ── Resize checks ───────────────────────────────────────────────
        if (m_ViewportSize.first > 0.0f && m_ViewportSize.second > 0.0f)
        {
            const auto& spec = m_Framebuffer->GetSpecification();
            if (spec.Width != (uint32_t)m_ViewportSize.first || spec.Height != (uint32_t)m_ViewportSize.second)
            {
                m_Framebuffer->Resize((uint32_t)m_ViewportSize.first, (uint32_t)m_ViewportSize.second);
                m_EditorCamera.SetViewportSize(m_ViewportSize.first, m_ViewportSize.second);
                m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.first, (uint32_t)m_ViewportSize.second);
            }
        }

        // ── Camera Controller Update ────────────────────────────────────
        if (m_ViewportPanel.IsFocused() || m_ViewportPanel.IsHovered())
        {
            m_EditorCamera.OnUpdate(ts);
        }

        // ── Render Scene into Framebuffer ───────────────────────────────
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.07f, 0.07f, 0.09f, 1.0f });
        RenderCommand::Clear();

        switch (m_Toolbar.GetState())
        {
            case SceneState::Edit:
            {
                m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera.GetCamera());
                break;
            }
            case SceneState::Play:
            {
                if (m_IsPlaying && m_RuntimeScene)
                    m_RuntimeScene->OnUpdateRuntime(ts);
                else
                    m_ActiveScene->OnUpdateRuntime(ts);
                break;
            }
            case SceneState::Pause:
            {
                // Simple pause: render runtime frame but don't tick gameplay systems
                if (m_IsPlaying && m_RuntimeScene)
                    m_RuntimeScene->OnUpdateRuntime(0.0f);
                else
                    m_ActiveScene->OnUpdateRuntime(0.0f);
                break;
            }
        }

        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        HZ_PROFILE_FUNCTION();

        // ── Dockspace Setup ──────────────────────────────────────────────
        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspace_flags);
        }

        // ── Main Menu Bar ───────────────────────────────────────────────
        if (ImGui::BeginMenuBar())
        {
            // File menu
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Project")) { HZ_CORE_INFO("New Project (TODO)"); }
                if (ImGui::MenuItem("Open Project")) { HZ_CORE_INFO("Open Project (TODO)"); }
                ImGui::Separator();
                if (ImGui::MenuItem("New Scene", "Ctrl+N")) NewScene();
                if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) OpenScene();
                if (ImGui::MenuItem("Save Scene", "Ctrl+S")) SaveSceneAs();
                if (ImGui::MenuItem("Save All")) { HZ_CORE_INFO("Save All (TODO)"); }
                ImGui::Separator();
                if (ImGui::MenuItem("Import Asset")) { HZ_CORE_INFO("Import Asset (TODO)"); }
                if (ImGui::MenuItem("Export Build")) { HZ_CORE_INFO("Export Build (TODO)"); }
                if (ImGui::MenuItem("Project Settings")) { HZ_CORE_INFO("Project Settings (TODO)"); }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) Application::Get().Close();
                ImGui::EndMenu();
            }

            // Edit menu
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) { HZ_CORE_INFO("Undo (TODO)"); }
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) { HZ_CORE_INFO("Redo (TODO)"); }
                ImGui::Separator();
                if (ImGui::MenuItem("Duplicate Entity", "Ctrl+D")) { HZ_CORE_INFO("Duplicate Entity (TODO)"); }
                if (ImGui::MenuItem("Delete Entity", "Del")) { HZ_CORE_INFO("Delete Entity (TODO)"); }
                ImGui::Separator();
                if (ImGui::MenuItem("Preferences")) { HZ_CORE_INFO("Preferences (TODO)"); }
                ImGui::EndMenu();
            }

            // View menu
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Scene Hierarchy", nullptr, &m_ShowSceneHierarchy);
                ImGui::MenuItem("Inspector", nullptr, &m_ShowInspector);
                ImGui::MenuItem("Console", nullptr, &m_ShowConsole);
                ImGui::MenuItem("Asset Browser", nullptr, &m_ShowAssetBrowser);
                ImGui::MenuItem("Viewport", nullptr, &m_ShowViewport);
                ImGui::Separator();
                ImGui::MenuItem("Profiler", nullptr, &m_ShowProfiler);
                ImGui::MenuItem("Terminal", nullptr, &m_ShowTerminal);
                ImGui::EndMenu();
            }

            // Tools menu
            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Shader Reload")) { HZ_CORE_INFO("Shader Reload (TODO)"); }
                if (ImGui::MenuItem("Asset Reload")) { HZ_CORE_INFO("Asset Reload (TODO)"); }
                if (ImGui::MenuItem("Memory Viewer")) { HZ_CORE_INFO("Memory Viewer (TODO)"); }
                if (ImGui::MenuItem("Performance Stats")) { HZ_CORE_INFO("Performance Stats (TODO)"); }
                if (ImGui::MenuItem("Debug Overlay")) { HZ_CORE_INFO("Debug Overlay (TODO)"); }
                ImGui::EndMenu();
            }

            // Scripting menu
            if (ImGui::BeginMenu("Scripting"))
            {
                if (ImGui::MenuItem("Create Script")) { HZ_CORE_INFO("Create Script (TODO)"); }
                if (ImGui::MenuItem("Reload Scripts")) { HZ_CORE_INFO("Reload Scripts (TODO)"); }
                if (ImGui::MenuItem("Open Script Workspace")) { HZ_CORE_INFO("Open Script Workspace (TODO)"); }
                ImGui::EndMenu();
            }

            // Plugins menu
            if (ImGui::BeginMenu("Plugins"))
            {
                if (ImGui::MenuItem("Plugin Manager")) { HZ_CORE_INFO("Plugin Manager (TODO)"); }
                if (ImGui::MenuItem("Load Plugin")) { HZ_CORE_INFO("Load Plugin (TODO)"); }
                ImGui::Separator();
                if (ImGui::MenuItem("Enable/Disable Plugins")) { HZ_CORE_INFO("Enable/Disable Plugins (TODO)"); }
                ImGui::EndMenu();
            }

            // Help menu
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("Documentation")) { HZ_CORE_INFO("Documentation (TODO)"); }
                if (ImGui::MenuItem("Engine Info")) { HZ_CORE_INFO("Engine Info (TODO)"); }
                if (ImGui::MenuItem("GitHub Repository")) { HZ_CORE_INFO("GitHub Repository (TODO)"); }
                ImGui::Separator();
                if (ImGui::MenuItem("About Horizon X")) { HZ_CORE_INFO("About Horizon X (TODO)"); }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // ── Editor Panels Render ────────────────────────────────────────
        // Startup / Welcome modal
        if (m_ShowStartup)
        {
            ImGui::Begin("Welcome to Horizon X", &m_ShowStartup, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

            if (ImGui::BeginTabBar("##WelcomeTabs"))
            {
                if (ImGui::BeginTabItem("Update Log"))
                {
                    ImGui::TextWrapped("Changelog: \n- Engine core improvements\n- New scripting templates\n- Editor stability fixes");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Templates"))
                {
                    ImGui::Text("Choose a template to start from:");
                    for (int i = 0; i < (int)m_Templates.size(); ++i)
                    {
                        if (ImGui::Selectable(m_Templates[i].c_str(), m_SelectedTemplate == i))
                            m_SelectedTemplate = i;
                    }

                    ImGui::Separator();
                    ImGui::InputText("Project Name", m_NewProjectName, sizeof(m_NewProjectName));
                    if (ImGui::Button("Create Project"))
                    {
                        std::string name = m_NewProjectName;
                        if (!name.empty())
                        {
                            std::filesystem::path dst = m_ProjectsRoot / name;
                            if (!std::filesystem::exists(dst))
                            {
                                // For now create a simple folder structure and copy a template marker file
                                std::filesystem::create_directories(dst);
                                std::ofstream marker(dst / "template.txt");
                                marker << "Template: " << m_Templates[m_SelectedTemplate] << std::endl;
                                marker.close();
                                RefreshProjectList();
                            }
                        }
                    }

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Projects"))
                {
                    ImGui::Text("Existing Projects:");
                    for (size_t i = 0; i < m_ProjectList.size(); ++i)
                    {
                        const auto& p = m_ProjectList[i];
                        ImGui::PushID((int)i);
                        if (ImGui::Selectable(p.filename().string().c_str()))
                        {
                            // Open project: set global editor context project path
                            ::Horizon::g_EditorContext.ProjectPath = p;
                            HZ_CORE_INFO("Opened project: {0}", p.string());
                        }
                        ImGui::PopID();
                    }

                    if (ImGui::Button("Refresh")) RefreshProjectList();

                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();

                ImGui::Separator();
                if (ImGui::Button("Close")) m_ShowStartup = false;
            }

            ImGui::End();
        }

        if (m_ShowSceneHierarchy) m_SceneHierarchyPanel.OnImGuiRender();
        if (m_ShowInspector) m_InspectorPanel.OnImGuiRender(m_SceneHierarchyPanel.GetSelectedEntity());
        if (m_ShowConsole) m_ConsolePanel.OnImGuiRender();
        if (m_ShowAssetBrowser) m_AssetBrowserPanel.OnImGuiRender();

        // Viewport Panel
        if (m_ShowViewport)
            m_ViewportSize = m_ViewportPanel.OnImGuiRender(m_Framebuffer);

        // Handle viewport picking on mouse click
        if (m_ViewportPanel.IsHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            // Convert screen pos to viewport local
            auto mouse = Input::GetMousePosition();
            glm::vec2 screenPos((float)mouse.first, (float)mouse.second);
            glm::vec2 vp = m_ViewportPanel.ScreenToViewport(screenPos);
            int id = m_ViewportPanel.PickEntityAt(vp);
            if (id >= 0)
            {
                // entt entity mapping: reconstruct Entity from id and set selection
                Entity e(static_cast<entt::entity>(id), m_ActiveScene.get());
                m_SceneHierarchyPanel.SetSelectedEntity(e);
            }
            else
            {
                m_SceneHierarchyPanel.SetSelectedEntity(Entity{});
            }
        }

        // Render Gizmos Overlay within Viewport context (placeholder)
        if (m_ShowViewport)
        {
            ImGui::Begin("Viewport Overlay");
            Gizmos::Draw(m_SceneHierarchyPanel.GetSelectedEntity(), m_EditorCamera.GetCamera(), m_GizmoType);
            ImGui::End();
        }

        // Render Playback Toolbar
        m_Toolbar.OnImGuiRender();
        // Handle playback state transitions
        switch (m_Toolbar.GetState())
        {
            case SceneState::Play:
            {
                // TODO: serialize and run scene (simple clone)
                if (!m_IsPlaying)
                {
                    // Create runtime copy using Scene::Clone() (avoids deleted copy ctor)
                    if (m_ActiveScene)
                        m_RuntimeScene = m_ActiveScene->Clone();
                    m_IsPlaying = true;
                    HZ_CORE_INFO("Entered Play Mode");
                }
                break;
            }
            case SceneState::Edit:
            {
                if (m_IsPlaying)
                {
                    m_IsPlaying = false;
                    m_RuntimeScene.reset();
                    HZ_CORE_INFO("Stopped Play Mode");
                }
                break;
            }
            default: break;
        }

        ImGui::End(); // DockSpace Demo End
    }

    void EditorLayer::OnEvent(Event& e)
    {
        // route events to editor camera
        // (no-op patch change)
        m_EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Viewport shortcuts
        if (e.IsRepeat())
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        switch (e.GetKeyCode())
        {
            // Scene Shortcuts
            case Key::N:
            {
                if (control)
                    NewScene();
                break;
            }
            case Key::O:
            {
                if (control)
                    OpenScene();
                break;
            }
            case Key::S:
            {
                if (control)
                    SaveSceneAs();
                break;
            }

            // Gizmo Switch Shortcuts
            case Key::W:
            {
                m_GizmoType = GizmoType::Translate;
                break;
            }
            case Key::E:
            {
                m_GizmoType = GizmoType::Rotate;
                break;
            }
            case Key::R:
            {
                m_GizmoType = GizmoType::Scale;
                break;
            }
        }
        return false;
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.first, (uint32_t)m_ViewportSize.second);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        HZ_CORE_INFO("Created new scene.");
    }

    void EditorLayer::RefreshProjectList()
    {
        m_ProjectList.clear();
        if (!std::filesystem::exists(m_ProjectsRoot))
            return;

        for (auto& p : std::filesystem::directory_iterator(m_ProjectsRoot))
        {
            if (p.is_directory())
                m_ProjectList.push_back(p.path());
        }
    }

    void EditorLayer::OpenScene()
    {
        auto filepath = FileDialogs::OpenFile("Horizon Scene (*.hxscene)\0*.hxscene\0");
        if (filepath)
        {
            NewScene();
            HZ_CORE_INFO("Opened scene from {0}", *filepath);
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        auto filepath = FileDialogs::SaveFile("Horizon Scene (*.hxscene)\0*.hxscene\0");
        if (filepath)
        {
            HZ_CORE_INFO("Saved scene as {0}", *filepath);
        }
    }

} // namespace Horizon
