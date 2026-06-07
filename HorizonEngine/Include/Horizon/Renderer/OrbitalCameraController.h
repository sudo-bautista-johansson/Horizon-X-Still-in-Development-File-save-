#pragma once

#include "Horizon/Renderer/PerspectiveCamera.h"
#include "Horizon/Core/Timestep.h"
#include "Horizon/Core/Events.h"

namespace Horizon {

	class OrbitalCameraController
	{
	public:
		OrbitalCameraController(float fov, float aspect, const glm::vec3& target = glm::vec3(0.0f));

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		PerspectiveCamera& GetCamera() { return m_Camera; }

	private:
		bool OnMouseMoved(class MouseMovedEvent& e);
		bool OnMouseScrolled(class MouseScrolledEvent& e);

	private:
		PerspectiveCamera m_Camera;
		glm::vec3 m_Target;
		float m_Distance = 10.0f;
		float m_Azimuth = 0.0f;
		float m_Polar = 45.0f; // degrees
		float m_Sensitivity = 0.25f;
	};

} // namespace Horizon
