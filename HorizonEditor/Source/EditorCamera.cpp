#include "EditorCamera.h"
#include "Horizon/Core/Timestep.h"
#include "Horizon/Core/Input.h"
#include "Horizon/Core/KeyCodes.h"
#include "Horizon/Core/MouseCodes.h"
#include <imgui.h>

namespace Horizon {

EditorCamera::EditorCamera(float fov, float aspect, float nearClip, float farClip)
	: m_FOV(fov), m_Aspect(aspect), m_Near(nearClip), m_Far(farClip)
{
	UpdateProjection();
	UpdateView();
}

void EditorCamera::SetViewportSize(float width, float height)
{
	m_Aspect = width / height;
	// Update orthographic camera projection
	float orthoHalfWidth = (width / height) * 10.0f;
	m_Camera.SetProjection(-orthoHalfWidth, orthoHalfWidth, -10.0f, 10.0f);
}

void EditorCamera::OnUpdate(Timestep ts)
{
	float delta = ts.GetSeconds();

	// Movement (WASD)
	float speed = m_MoveSpeed;
	if (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift))
		speed *= 4.0f;

	glm::vec3 forward;
	forward.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	forward.y = sin(glm::radians(m_Pitch));
	forward.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	forward = glm::normalize(forward);
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

	if (Input::IsKeyPressed(Key::W)) m_Position += forward * speed * delta;
	if (Input::IsKeyPressed(Key::S)) m_Position -= forward * speed * delta;
	if (Input::IsKeyPressed(Key::A)) m_Position -= right * speed * delta;
	if (Input::IsKeyPressed(Key::D)) m_Position += right * speed * delta;

	// Mouse rotation/pan when over viewport
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
	{
		// Right mouse = rotate
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			double mx, my;
			auto mp = Input::GetMousePosition();
			mx = mp.first; my = mp.second;
			if (m_FirstMouse)
			{
				m_LastX = mx; m_LastY = my; m_FirstMouse = false;
			}

			float xoffset = (float)(mx - m_LastX);
			float yoffset = (float)(m_LastY - my);
			m_LastX = mx; m_LastY = my;

			xoffset *= m_Sensitivity;
			yoffset *= m_Sensitivity;

			m_Yaw += xoffset;
			m_Pitch += yoffset;
			if (m_Pitch > 89.0f) m_Pitch = 89.0f;
			if (m_Pitch < -89.0f) m_Pitch = -89.0f;
		}
		else
		{
			m_FirstMouse = true;
		}

		// Middle mouse = pan
		if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
		{
			auto mp = Input::GetMousePosition();
			double mx = mp.first, my = mp.second;
			if (m_FirstMouse)
			{
				m_LastX = mx; m_LastY = my; m_FirstMouse = false;
			}
			float xoffset = (float)(mx - m_LastX);
			float yoffset = (float)(my - m_LastY);
			m_LastX = mx; m_LastY = my;

			// translate in screen plane
			m_Position -= glm::vec3(xoffset * 0.01f, -yoffset * 0.01f, 0.0f);
		}
		else
		{
			// do not reset m_FirstMouse here because right mouse may also be used
		}
	}

	// Mouse wheel zoom (use ImGui IO mouse wheel)
	if (io.MouseWheel != 0.0f)
	{
		m_Position += glm::vec3(0.0f, 0.0f, -io.MouseWheel * 0.5f);
	}

	UpdateView();
	// sync camera transform
	m_Camera.SetPosition(m_Position);
	m_Camera.SetRotation(m_Rotation.y);
}

void EditorCamera::UpdateProjection()
{
	m_Projection = glm::perspective(glm::radians(m_FOV), m_Aspect, m_Near, m_Far);
	m_ViewProjection = m_Projection * m_View;
}

void EditorCamera::UpdateView()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front = glm::normalize(front);

	glm::mat4 view = glm::lookAt(m_Position, m_Position + front, glm::vec3(0.0f, 1.0f, 0.0f));
	m_View = view;
	m_ViewProjection = m_Projection * m_View;
}

} // namespace Horizon
