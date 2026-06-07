#pragma once

#include "Horizon/Renderer/PerspectiveCamera.h"
#include "Horizon/Core/Timestep.h"
#include "Horizon/Core/Events.h"

namespace Horizon {

	class FreeCameraController
	{
	public:
		FreeCameraController(float fov, float aspect, float nearClip = 0.1f, float farClip = 1000.0f);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		PerspectiveCamera& GetCamera() { return m_Camera; }

	private:
		bool OnMouseMoved(class MouseMovedEvent& e);
		bool OnMouseScrolled(class MouseScrolledEvent& e);

	private:
		PerspectiveCamera m_Camera;
		float m_MovementSpeed = 5.0f;
		float m_Sensitivity = 0.1f;
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
		glm::vec3 m_Position{0.0f};
		glm::vec3 m_Front{0.0f, 0.0f, -1.0f};
		glm::vec3 m_Up{0.0f, 1.0f, 0.0f};
		bool m_FirstMouse = true;
		double m_LastX = 0.0, m_LastY = 0.0;
	};

} // namespace Horizon

