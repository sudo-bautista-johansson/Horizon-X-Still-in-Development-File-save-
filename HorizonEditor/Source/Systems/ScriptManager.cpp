#include "ScriptManager.h"
#include "../Scripting/ScriptingSystem.h"
#include "Horizon/Core/Log.h"
// No-op: ensure includes are present

namespace Horizon {

std::function<void(const std::filesystem::path&)> ScriptManager::s_OpenCallback;

void ScriptManager::Init()
{
}

void ScriptManager::Shutdown()
{
}

std::filesystem::path ScriptManager::CreateScript(const std::filesystem::path& directory, const std::string& name)
{
	auto path = ScriptingSystem::Get().CreateScript(directory, name);
	// Notify open callback if set
	if (s_OpenCallback) s_OpenCallback(path);
	HZ_CORE_INFO("ScriptManager: Created script {0}", path.string());
	return path;
}

void ScriptManager::SetOpenCallback(std::function<void(const std::filesystem::path&)> cb)
{
	s_OpenCallback = std::move(cb);
}

} // namespace Horizon
