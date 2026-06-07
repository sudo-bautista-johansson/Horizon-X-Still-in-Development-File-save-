#include "Horizon/Assets/AssetManager.h"
#include "Horizon/Renderer/Texture.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace Horizon {

	static std::queue<std::string> s_Requests;
	static std::mutex s_Mutex;
	static std::condition_variable s_CV;
	static bool s_Running = false;

	static std::thread s_Worker;

	void AssetManager::Init()
	{
		s_Running = true;
		s_Worker = std::thread([](){
			while (s_Running)
			{
				std::unique_lock<std::mutex> lock(s_Mutex);
				s_CV.wait(lock, []{ return !s_Requests.empty() || !s_Running; });
				if (!s_Running) break;
				auto path = s_Requests.front(); s_Requests.pop();
				lock.unlock();

				// Load texture synchronously into cache (simple approach)
				// Could instead push raw data into a staging queue for main-thread upload
				auto tex = Texture2D::Create(path);
				// Store somewhere: for now we rely on Texture2D Create to manage resources
			}
		});
	}

	void AssetManager::Shutdown()
	{
		s_Running = false;
		s_CV.notify_all();
		if (s_Worker.joinable()) s_Worker.join();
	}

	void AssetManager::QueueTextureLoad(const std::string& path)
	{
		std::lock_guard<std::mutex> lock(s_Mutex);
		s_Requests.push(path);
		s_CV.notify_one();
	}

} // namespace Horizon
