#include "Horizon/Assets/AssetManager.h"
#include <fstream>

namespace Horizon {

	std::unordered_map<std::string, std::string> AssetManager::s_TextCache;

	void AssetManager::Init()
	{
	}

	void AssetManager::QueueTextureLoad(const std::string& path)
	{
		// For now, synchronous quick path
		// In future, delegate to async loader
		(void)path;
	}

	void AssetManager::Shutdown()
	{
		// nothing for now
	}

	std::string AssetManager::LoadTextFile(const std::string& path)
	{
		auto it = s_TextCache.find(path);
		if (it != s_TextCache.end())
			return it->second;

		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (!in) return {};
		in.seekg(0, std::ios::end);
		std::string data;
		data.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&data[0], data.size());
		in.close();
		s_TextCache[path] = data;
		return data;
	}

} // namespace Horizon
