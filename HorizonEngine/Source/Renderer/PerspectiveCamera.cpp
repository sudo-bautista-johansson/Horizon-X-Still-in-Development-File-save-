#include "Horizon/Renderer/PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Horizon {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_Near(nearClip), m_Far(farClip)
	{
		SetProjection(fov, aspectRatio, nearClip, farClip);
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_FOV = fov; m_AspectRatio = aspectRatio; m_Near = nearClip; m_Far = farClip;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1,0,0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0,1,0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0,0,1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

} // namespace Horizon
