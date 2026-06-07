#include "Horizon/Renderer/OrbitalCameraController.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Horizon/Core/Input.h"

namespace Horizon {

	OrbitalCameraController::OrbitalCameraController(float fov, float aspect, const glm::vec3& target)
		: m_Camera(fov, aspect, 0.1f, 1000.0f), m_Target(target)
	{
	}

	void OrbitalCameraController::OnUpdate(Timestep ts)
	{
		// Recompute camera position
		float radPolar = glm::radians(m_Polar);
		float radAzim = glm::radians(m_Azimuth);

		glm::vec3 pos;
		pos.x = m_Target.x + m_Distance * sin(radPolar) * cos(radAzim);
		pos.y = m_Target.y + m_Distance * cos(radPolar);
		pos.z = m_Target.z + m_Distance * sin(radPolar) * sin(radAzim);

		m_Camera.SetPosition(pos);
		// Look at target
		m_Camera.SetRotation(glm::vec3(0.0f));
	}

	void OrbitalCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& ev){ return OnMouseMoved(ev); });
		dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& ev){ return OnMouseScrolled(ev); });
	}

	bool OrbitalCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		// Simple approach: use mouse delta compared to center or previous stored
		static float lastX = e.GetX();
		static float lastY = e.GetY();
		float xoffset = e.GetX() - lastX;
		float yoffset = e.GetY() - lastY;
		lastX = e.GetX(); lastY = e.GetY();

		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			m_Azimuth += xoffset * m_Sensitivity;
			m_Polar += yoffset * m_Sensitivity;
			m_Polar = glm::clamp(m_Polar, 1.0f, 179.0f);
		}
		return false;
	}

	bool OrbitalCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Distance -= e.GetYOffset();
		m_Distance = std::max(0.1f, m_Distance);
		return false;
	}

} // namespace Horizon
