#pragma once

#include <string>
#include <array>

namespace Launcher {

struct ProjectConfig
{
	std::string Name;
	std::string EngineVersion;
	std::string Template;
	std::array<int,2> Resolution{};
};

} // namespace Launcher
