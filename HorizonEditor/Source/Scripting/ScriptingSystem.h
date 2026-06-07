#pragma once

#include "Horizon/Core/Base.h"
#include <filesystem>
#include <functional>

namespace Horizon {

class ScriptingSystem
{
public:
	static ScriptingSystem& Get();

	// Create a new Lua script with a template. Returns the created path.
	std::filesystem::path CreateScript(const std::filesystem::path& directory, const std::string& name);

	// Open script in editor (invokes callback)
	void OpenScriptInEditor(const std::filesystem::path& path);

	// Set callback that will be invoked when a script should be opened in the editor
	void SetOpenCallback(std::function<void(const std::filesystem::path&)> cb) { m_OpenCallback = std::move(cb); }

	// Reload scripting runtime (stub for now)
	void ReloadScripts();

private:
	ScriptingSystem() = default;

	std::function<void(const std::filesystem::path&)> m_OpenCallback;
};

} // namespace Horizon
