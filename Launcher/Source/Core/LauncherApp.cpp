#include "LauncherApp.h"

#include <glad/glad.h>
// Prevent GLFW from including the OpenGL headers itself (avoid conflict with glad)
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <memory>
#include <filesystem>
#include <vector>
#include <thread>

#include "../Systems/ProjectManager.h"
#include "../Systems/TemplateManager.h"
#include "../Systems/UpdateManager.h"

namespace fs = std::filesystem;

namespace Launcher {

struct LauncherApp::Impl
{
	std::unique_ptr<ProjectManager> ProjectMgr;
	std::unique_ptr<TemplateManager> TemplateMgr;
	std::unique_ptr<UpdateManager> UpdateMgr;
	bool running = true;
};

LauncherApp::LauncherApp(const std::string& name)
	: m_Impl(new Impl())
{
	fs::create_directories("Launcher/Data/Updates");
	fs::create_directories("Launcher/Data/Templates");
	fs::create_directories("Launcher/Data/Projects");

	m_Impl->ProjectMgr = std::make_unique<ProjectManager>("Launcher/Data/Projects/projects.db");
	m_Impl->TemplateMgr = std::make_unique<TemplateManager>("Launcher/Data/Templates");
	m_Impl->UpdateMgr = std::make_unique<UpdateManager>("Launcher/Data/Updates");
}

LauncherApp::~LauncherApp()
{
	delete m_Impl;
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void LauncherApp::Run()
{
	// Initialize GLFW + OpenGL + ImGui
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) return;

	// GL 3.3
	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Horizon X Launcher", NULL, NULL);
	if (window == NULL) return;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD\n");
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// UI state
	char projectName[128] = "NewProject";
	char projectPath[1024] = "";
	int resW = 1280, resH = 720;
	int selectedTemplate = 0;

	while (!glfwWindowShouldClose(window) && m_Impl->running)
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Horizon X Launcher");

		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Update Log"))
			{
				auto updates = m_Impl->UpdateMgr->GetUpdates();
				for (auto& u : updates)
				{
					ImGui::Text("%s - %s", u.Version.c_str(), u.Date.c_str());
					ImGui::Separator();
					ImGui::TextWrapped("%s", u.Content.c_str());
					ImGui::Spacing();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Templates"))
			{
				auto templates = m_Impl->TemplateMgr->GetAvailableTemplates();
				ImGui::Text("Create Project");
				ImGui::InputText("Name", projectName, IM_ARRAYSIZE(projectName));
				ImGui::InputText("Path", projectPath, IM_ARRAYSIZE(projectPath));
				ImGui::InputInt("Res W", &resW);
				ImGui::InputInt("Res H", &resH);
				if (!templates.empty())
				{
					const char* items[64];
					int count = (int)templates.size();
					for (int i=0;i<count && i<64;i++) items[i] = templates[i].c_str();
					ImGui::Combo("Template", &selectedTemplate, items, count);
				}
				if (ImGui::Button("Create Project"))
				{
					ProjectConfig cfg;
					cfg.Name = projectName;
					cfg.EngineVersion = "0.2.1";
					cfg.Template = (templates.empty()?"Empty Project":templates[selectedTemplate]);
					cfg.Resolution = {resW, resH};
					std::string path = projectPath;
					if (path.empty()) path = std::string("./") + cfg.Name;
					if (m_Impl->TemplateMgr->CreateProjectFromTemplate(cfg.Template, path, cfg))
					{
						m_Impl->ProjectMgr->RegisterProject(path);
					}
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Projects"))
			{
				auto projects = m_Impl->ProjectMgr->GetAllProjects();
				ImGui::BeginChild("proj_list", ImVec2(0,300), true);
				for (size_t i=0;i<projects.size();++i)
				{
					auto& p = projects[i];
					ImGui::Text("%s", p.Name.c_str());
					ImGui::SameLine();
					if (ImGui::SmallButton((std::string("Open##")+std::to_string(i)).c_str()))
					{
						std::string editorExe = "HorizonEditor/Debug/HorizonEditor.exe";
						std::string arg = " --project=\"" + p.Path + "\"";
						std::string command = editorExe + arg;
						// launch in background
						std::thread([command](){ std::system(command.c_str()); }).detach();
					}
					ImGui::Separator();
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}

LauncherApp* CreateLauncherApp()
{
	return new LauncherApp();
}

} // namespace Launcher
