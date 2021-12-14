#pragma once

const float SENSITIVITY =  0.1f;
const float SPEED = 10.0f;
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ZOOM = 45.0f;

enum class CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
	glm::vec3 m_position, m_front, m_right, m_up, m_worldUp;
	float m_sensitivity, m_speed, m_yaw, m_pitch, m_zoom;

public:
	Camera(glm::vec3 position);
	

	void KeyInput(const CameraMovement& cm, float deltaTime);
	void CursorInput(float xOffset, float yOffset);
	void WheelInput(double yOffset);

	glm::vec3 GetPosition();
	glm::vec3 GetFront();
	glm::mat4 GetView();
	const float& GetZoom() const;

private:
	void UpdateCamera();
	void Setup();


};


