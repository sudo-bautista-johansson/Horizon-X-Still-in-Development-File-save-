#pragma once

#include "Horizon/Core/Timestep.h"
#include "Horizon/Renderer/GLM.h"
#include "Horizon/Core/Input.h"
#include "Horizon/Core/KeyCodes.h"
#include "Horizon/Core/MouseCodes.h"
#include "Horizon/Renderer/OrthographicCamera.h"

namespace Horizon {

class EditorCamera
{
public:
	EditorCamera(float fov = 45.0f, float aspect = 16.0f/9.0f, float nearClip = 0.1f, float farClip = 1000.0f);

	void OnUpdate(Timestep ts);
	void OnEvent(Event& e) { /* optional */ }

	void SetViewportSize(float width, float height);

	const glm::vec3& GetPosition() const { return m_Position; }
	const OrthographicCamera& GetCamera() const { return m_Camera; }

private:
	void UpdateProjection();
	void UpdateView();

private:
	float m_FOV, m_Aspect, m_Near, m_Far;
	glm::vec3 m_Position{ 0.0f };
	glm::vec3 m_Rotation{ 0.0f };
	float m_Yaw = -90.0f, m_Pitch = 0.0f;
	float m_MoveSpeed = 5.0f;
	float m_Sensitivity = 0.1f;

	glm::mat4 m_View{1.0f};
	glm::mat4 m_Projection{1.0f};
	glm::mat4 m_ViewProjection{1.0f};

	OrthographicCamera m_Camera{ -10.0f, 10.0f, -10.0f, 10.0f };

	double m_LastX = 0.0, m_LastY = 0.0;
	bool m_FirstMouse = true;
};

} // namespace Horizon

