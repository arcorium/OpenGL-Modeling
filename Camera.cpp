#include "pch.h"
#include "Camera.h"

Camera::Camera(glm::vec3 position)
	:m_position(position), m_front(glm::vec3{0.0f, 0.0f, -1.0f}), m_right(), m_up(), m_worldUp(glm::vec3{0.0f, 1.0f, 0.0f}),
		m_sensitivity(SENSITIVITY), m_speed(SPEED), m_yaw(YAW), m_pitch(PITCH), m_zoom(ZOOM)
{
	Setup();
}

void Camera::Setup()
{
	UpdateCamera();
}

void Camera::KeyInput(const CameraMovement& cm, float deltaTime)
{
	float velocity = deltaTime * m_speed;
	if (cm == CameraMovement::FORWARD)
		m_position += m_front * velocity;
	if (cm == CameraMovement::BACKWARD)
		m_position -= m_front * velocity;
	if (cm == CameraMovement::LEFT)
		m_position -= m_right * velocity;
	if (cm == CameraMovement::RIGHT)
		m_position += m_right * velocity;

}

void Camera::CursorInput(float xOffset, float yOffset)
{
	xOffset *= m_sensitivity;
	yOffset *= m_sensitivity;

	m_yaw += xOffset;
	m_pitch -= yOffset;	// When mouse go up the value will be (-)


	// Constraint
	m_pitch = std::min(m_pitch, 89.0f);
	m_pitch = std::max(m_pitch, -89.0f);

	UpdateCamera();
}

void Camera::WheelInput(double yOffset)
{
	m_zoom -= static_cast<float>(yOffset);

	m_zoom = std::max(m_zoom, 0.0f);
	m_zoom = std::min(m_zoom, 45.0f);
}

void Camera::UpdateCamera()
{
	glm::vec3 front;
	front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
	front.y = std::sin(glm::radians(m_pitch));
	front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	// Adjust right and up vector
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::vec3 Camera::GetPosition()
{
	return m_position;
}

glm::vec3 Camera::GetFront()
{
	return m_front;
}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(m_position, m_front + m_position, m_up);
}

const float& Camera::GetZoom() const
{
	return m_zoom;
}
