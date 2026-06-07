#pragma once

#include <string>

namespace Launcher {

class LauncherApp
{
public:
	LauncherApp(const std::string& name = "Horizon X Launcher");
	~LauncherApp();

	void Run();

private:
	struct Impl;
	Impl* m_Impl;
};

LauncherApp* CreateLauncherApp();

} // namespace Launcher
