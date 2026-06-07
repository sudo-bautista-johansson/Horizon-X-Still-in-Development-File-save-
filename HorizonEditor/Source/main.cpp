#include "Horizon/Core/Application.h"
#include "EditorLayer.h"

namespace Horizon {

    class EditorApp : public Application
    {
    public:
        EditorApp()
            : Application("Horizon X Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~EditorApp() override
        {
        }
    };

    Application* CreateApplication()
    {
        return new EditorApp();
    }

} // namespace Horizon

int main(int argc, char** argv)
{
    auto app = Horizon::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
