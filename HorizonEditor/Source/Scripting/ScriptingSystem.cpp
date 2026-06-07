#include "ScriptingSystem.h"
#include <fstream>
#include "Horizon/Core/Log.h"

namespace Horizon {

ScriptingSystem& ScriptingSystem::Get()
{
	static ScriptingSystem instance;
	return instance;
}

std::filesystem::path ScriptingSystem::CreateScript(const std::filesystem::path& directory, const std::string& name)
{
	std::filesystem::create_directories(directory);
	auto path = directory / (name + ".lua");

	if (std::filesystem::exists(path))
	{
		HZ_CORE_WARN("Script already exists: {0}", path.string());
		return path;
	}

	std::ofstream ofs(path);
	if (!ofs)
	{
		HZ_CORE_ERROR("Failed to create script: {0}", path.string());
		return path;
	}

	ofs << "-- " << name << " - Horizon X Script\n";
	ofs << "local M = {}\n\n";
	ofs << "function M:OnCreate()\n    -- Initialization\nend\n\n";
	ofs << "function M:OnUpdate(dt)\n    -- Update logic\nend\n\n";
	ofs << "return M\n";
	ofs.close();

	HZ_CORE_INFO("Created script: {0}", path.string());

	// If an editor callback is set, open it
	if (m_OpenCallback) m_OpenCallback(path);

	return path;
}

void ScriptingSystem::OpenScriptInEditor(const std::filesystem::path& path)
{
	if (m_OpenCallback) m_OpenCallback(path);
}

void ScriptingSystem::ReloadScripts()
{
	HZ_CORE_INFO("Scripting: ReloadScripts called (stub)");
}

} // namespace Horizon
