#pragma once

#include "Horizon/Renderer/Camera.h"
#include "Horizon/Renderer/GLM.h"

namespace Horizon {

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void SetProjection(float fov, float aspectRatio, float nearClip, float farClip);

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		const glm::vec3& GetPosition() const { return m_Position; }

		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		const glm::vec3& GetRotation() const { return m_Rotation; }

		const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position{0.0f};
		glm::vec3 m_Rotation{0.0f};

		float m_FOV = 45.0f;
		float m_AspectRatio = 1.7777f;
		float m_Near = 0.1f;
		float m_Far = 1000.0f;
	};

} // namespace Horizon
