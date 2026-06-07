#pragma once

#include "Horizon/Renderer/GLM.h"

namespace Horizon {

	class Camera
	{
	public:
		virtual ~Camera() = default;
		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
	};

} // namespace Horizon
