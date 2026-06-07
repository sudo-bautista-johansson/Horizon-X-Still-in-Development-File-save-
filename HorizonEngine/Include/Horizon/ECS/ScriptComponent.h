#pragma once

#include "Horizon/Core/Base.h"
#include <string>

namespace Horizon {

	struct ScriptComponent
	{
		std::string ScriptPath;
		ScriptComponent() = default;
		ScriptComponent(const std::string& path) : ScriptPath(path) {}
	};

} // namespace Horizon
