#include "Core/LauncherApp.h"

int main(int argc, char** argv)
{
	auto app = Launcher::CreateLauncherApp();
	app->Run();
	delete app;
	return 0;
}
