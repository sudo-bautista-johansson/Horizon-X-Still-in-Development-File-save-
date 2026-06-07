#pragma once

#include "Horizon/Core/Base.h"
#include <filesystem>
#include <functional>

namespace Horizon {

class ScriptManager
{
public:
	static void Init();
	static void Shutdown();

	// Create script via manager: create file, register asset and notify callback
	static std::filesystem::path CreateScript(const std::filesystem::path& directory, const std::string& name);

	// Callback when a script asset is created/opened
	static void SetOpenCallback(std::function<void(const std::filesystem::path&)> cb);

private:
	static std::function<void(const std::filesystem::path&)> s_OpenCallback;
};

} // namespace Horizon
