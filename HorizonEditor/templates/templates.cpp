#include "templates.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <Horizon/Core/Log.h>

using namespace Horizon;

std::vector<std::string> TemplateManager::GetAvailableTemplates()
{
	return { "Shooter 2D", "RPG 2D", "Isometric 2D", "Top-Down 2D", "Side-Scroller 2D" };
}

bool TemplateManager::CopyDirectory(const std::filesystem::path& src, const std::filesystem::path& dst)
{
	try
	{
		if (!std::filesystem::exists(src) || !std::filesystem::is_directory(src))
			return false;

		if (std::filesystem::exists(dst))
			return false;

		std::filesystem::create_directories(dst);

		for (auto& entry : std::filesystem::recursive_directory_iterator(src))
		{
			const auto& path = entry.path();
			auto relative = std::filesystem::relative(path, src);
			auto dest = dst / relative;

			if (entry.is_directory())
				std::filesystem::create_directories(dest);
			else if (entry.is_regular_file())
				std::filesystem::copy_file(path, dest, std::filesystem::copy_options::overwrite_existing);
		}
		return true;
	}
	catch (const std::exception& e)
	{
		HZ_CORE_ERROR("Failed to copy template directory: {0}", e.what());
		return false;
	}
}

bool TemplateManager::CreateProjectFromTemplate(const std::string& templateName, const std::filesystem::path& dstPath)
{
	try
	{
		if (std::filesystem::exists(dstPath))
			return false;

		std::filesystem::path templatesRoot = std::filesystem::current_path() / "Templates";
		std::filesystem::path src = templatesRoot / templateName;

		if (std::filesystem::exists(src) && std::filesystem::is_directory(src))
		{
			if (!CopyDirectory(src, dstPath))
				return false;
		}
		else
		{
			// create minimal project structure
			std::filesystem::create_directories(dstPath / "Assets");
			std::filesystem::create_directories(dstPath / "Scripts");
			std::ofstream manifest(dstPath / "project.hzn");
			manifest << "name: " << templateName << "\n";
			manifest << "template: " << templateName << "\n";
			manifest.close();
		}

		HZ_CORE_INFO("Created project '{0}' from template '{1}'", dstPath.string(), templateName);
		return true;
	}
	catch (const std::exception& e)
	{
		HZ_CORE_ERROR("Failed to create project from template: {0}", e.what());
		return false;
	}
}

