#include "Horizon/Renderer/FreeCameraController.h"
#include "Horizon/Core/Input.h"
#include "Horizon/Core/KeyCodes.h"
#include "Horizon/Renderer/GLM.h"

using namespace Horizon;

FreeCameraController::FreeCameraController(float fov, float aspect, float nearClip, float farClip)
	: m_Camera(fov, aspect, nearClip, farClip)
{
}

void FreeCameraController::OnUpdate(Timestep ts)
{
	float delta = (float)ts;
	if (Input::IsKeyPressed(Key::W)) m_Position += m_Front * m_MovementSpeed * delta;
	if (Input::IsKeyPressed(Key::S)) m_Position -= m_Front * m_MovementSpeed * delta;
	if (Input::IsKeyPressed(Key::A)) m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * m_MovementSpeed * delta;
	if (Input::IsKeyPressed(Key::D)) m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * m_MovementSpeed * delta;

	m_Camera.SetPosition(m_Position);
}

void FreeCameraController::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& ev) { return OnMouseMoved(ev); });
	dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& ev) { return OnMouseScrolled(ev); });
}

bool FreeCameraController::OnMouseMoved(MouseMovedEvent& e)
{
	if (m_FirstMouse)
	{
		m_LastX = e.GetX(); m_LastY = e.GetY(); m_FirstMouse = false;
	}

	float xoffset = (float)(e.GetX() - m_LastX);
	float yoffset = (float)(m_LastY - e.GetY());
	m_LastX = e.GetX(); m_LastY = e.GetY();

	xoffset *= m_Sensitivity; yoffset *= m_Sensitivity;

	m_Yaw += xoffset; m_Pitch += yoffset;
	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);

	m_Camera.SetRotation(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	return false;
}

bool FreeCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	float offset = e.GetYOffset();
	m_MovementSpeed += offset * 0.5f;
	m_MovementSpeed = std::max(0.1f, m_MovementSpeed);
	return false;
}
