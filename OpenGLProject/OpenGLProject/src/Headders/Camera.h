#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/ext.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void update(float deltaTime);

	void setProjectionView(float a_fov, float a_aspect, float a_near, float a_far);
	inline void setPos(const glm::vec3& a_pos) { m_position = a_pos; }

	inline glm::vec3 GetPos() const { return m_position; }
	glm::mat4 GetProjectionView();
	inline glm::mat4 GetViewMat()const {return glm::lookAt(this->m_position, this->m_position + this->m_cameraFront, this->m_cameraUp);
	}


	void Lookat(glm::vec3 a_target);

private:
	void CalculateFront();

	glm::vec3 m_position;
	glm::vec3 m_cameraFront = glm::vec3(0, 0, 1);
	glm::vec3 m_cameraUp = glm::vec3(0, 1, 0);
	
	float m_cameraYaw = 0.0f;
	float m_cameraPitch = 0.0f;
	float m_cameraRoll = 0.0f;

	// camera transforms
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	float m_cameraMouseSensitivity = 0.6f;

	int m_lastMouseXPos, m_lastMouseYPos;
};

