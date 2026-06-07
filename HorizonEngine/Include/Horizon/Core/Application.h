#pragma once

#include "Horizon/Core/Base.h"
#include "Horizon/Core/Window.h"
#include "Horizon/Core/LayerStack.h"
#include "Horizon/Core/Events.h"
#include "Horizon/Core/Timestep.h"
#include "Horizon/ImGui/ImGuiLayer.h"

namespace Horizon {

    class Application
    {
    public:
        Application(const std::string& name = "Horizon X Engine");
        virtual ~Application();

        void Run();
        void Close();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        Window& GetWindow() { return *m_Window; }
        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        static Application& Get() { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

        static Application* s_Instance;
    };

    // Client defined
    Application* CreateApplication();

} // namespace Horizon
