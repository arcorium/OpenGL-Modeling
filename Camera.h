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
	RIGHT,
	KEYUP
};

class Camera
{
	glm::mat4 m_model, m_projection;
	glm::vec3 m_position, m_front, m_right, m_up, m_worldUp;
	float m_sensitivity, m_speed, m_yaw, m_pitch, m_zoom;

	glm::vec3 m_velocity;

public:
	Camera(glm::vec3 camPos, const glm::mat4& model = glm::mat4{1.0f},
		const glm::mat4& projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WINDOW_WIDTH / WINDOW_HEIGHT), 1.0f, 100.0f));
	

	void KeyInput(const CameraMovement& cm, float deltaTime);
	void Move();
	void CursorInput(float xOffset, float yOffset);
	void WheelInput(double yOffset);

	glm::vec3 GetPosition() const;
	glm::vec3 GetFront() const;
	glm::mat4 GetModel() const;
	glm::mat4 GetProjection() const;
	glm::mat4 GetView() const;
	const float& GetZoom() const;
	glm::mat4 GetMatrix();

	void SetModel(const glm::mat4& model);
	void SetProjection(const glm::mat4& proj);

private:
	void UpdateCamera();
	void Setup();


};


