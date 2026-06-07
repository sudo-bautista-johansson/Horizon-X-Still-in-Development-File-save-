#pragma once

#include "Horizon/Core/Timestep.h"
#include "Horizon/Core/Events.h"

namespace Horizon {

	class CameraController
	{
	public:
		virtual ~CameraController() = default;
		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;
	};

} // namespace Horizon
