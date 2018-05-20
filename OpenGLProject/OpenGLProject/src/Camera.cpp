#include "Camera.h"
#include <glm/glm.hpp>

#include "Input.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::update(float deltaTime) {
	Input* input = Input::getInstance();

	float speed = 5.0f;
	// move fword and back
	if (input->isKeyDown(INPUT_KEY_W)) {
		m_position += m_cameraFront * speed * deltaTime;
	}
	if (input->isKeyDown(INPUT_KEY_S)) {
		m_position -= m_cameraFront * speed * deltaTime;
	}

	// move left and right
	if (input->isKeyDown(INPUT_KEY_A)) {
		glm::vec3 dir = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
		m_position -= dir * speed * deltaTime;
	}
	if (input->isKeyDown(INPUT_KEY_D)) {
		glm::vec3 dir = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
		m_position += dir * speed * deltaTime;
	}

	// move up and down
	if (input->isKeyDown(INPUT_KEY_E)) {
		m_position.y += speed * deltaTime;
	}
	if (input->isKeyDown(INPUT_KEY_Q)) {
		m_position.y -= speed * deltaTime;
	}
	
	if (input->isMouseButtonDown(INPUT_MOUSE_BUTTON_RIGHT)) {
		int mouseXPos, mouseYPos;
		input->getMouseXY(&mouseXPos, &mouseYPos);

		float deltaMouseX = mouseXPos - m_lastMouseXPos;
		float deltaMouseY = mouseYPos - m_lastMouseYPos;

		m_cameraYaw += deltaMouseX * m_cameraMouseSensitivity;
		m_cameraPitch += deltaMouseY * m_cameraMouseSensitivity;

		if (m_cameraPitch > 85.0f) m_cameraPitch = 85.0f;
		if (m_cameraPitch <-85.0f) m_cameraPitch = -85.0f;

		CalculateFront();
	}

	m_viewMatrix = glm::lookAt(m_position, m_position + m_cameraFront, m_cameraUp);

	input->getMouseXY(&m_lastMouseXPos, &m_lastMouseYPos);


}

void Camera::setProjectionView(float a_fov, float a_aspect, float a_near, float a_far)
{
	m_projectionMatrix = glm::perspective(a_fov, a_aspect, a_near, a_far);
}

glm::mat4 Camera::GetProjectionView()
{
	return m_projectionMatrix * m_viewMatrix;
}

void Camera::Lookat(glm::vec3 a_target)
{
	glm::vec3 dir = glm::normalize(a_target - m_position);
	m_cameraPitch = glm::degrees(glm::asin(dir.y));
	m_cameraYaw = glm::degrees(atan2(dir.y, dir.x));

	CalculateFront();
}

void Camera::CalculateFront()
{
	glm::vec3 front;
	front.x = glm::cos(glm::radians(m_cameraYaw)) * glm::cos(glm::radians(m_cameraPitch));
	front.y = glm::sin(glm::radians(m_cameraPitch));
	front.z = glm::sin(glm::radians(m_cameraYaw)) * glm::cos(glm::radians(m_cameraPitch));
	m_cameraFront = glm::normalize(front);
}
