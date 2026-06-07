#pragma once

#include <string>
#include <unordered_map>

namespace Horizon {

	class AssetManager
	{
	public:
		static void Init();
		static void Shutdown();
		static std::string LoadTextFile(const std::string& path);
		static void QueueTextureLoad(const std::string& path);
	private:
		static std::unordered_map<std::string, std::string> s_TextCache;
	};

} // namespace Horizon
