#pragma once

#include "Horizon/Core/Base.h"
#include "Horizon/Renderer/Camera.h"
#include "Horizon/Renderer/Texture.h"
#include "Horizon/Renderer/GLM.h"

namespace Horizon {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();

		// Draw by transform
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture = nullptr, float tilingFactor = 1.0f, int entityID = -1);
	};

} // namespace Horizon
