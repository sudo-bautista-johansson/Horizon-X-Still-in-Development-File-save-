
#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace Horizon
{
	// Small manager for project templates. Provides available template names and
	// a function to create a new project from a template directory (if present)
	class TemplateManager
	{
	public:
		// Returns the list of available templates (fixed list by convention)
		static std::vector<std::string> GetAvailableTemplates();

		// Create a project at dstPath using the named template. Returns true on success.
		// If a folder templates/<templateName>/ exists it will be copied into dstPath,
		// otherwise a minimal project structure will be created and a marker file written.
		static bool CreateProjectFromTemplate(const std::string& templateName, const std::filesystem::path& dstPath);

	private:
		static bool CopyDirectory(const std::filesystem::path& src, const std::filesystem::path& dst);
	};

}
