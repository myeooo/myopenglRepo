#include "Camera.h"

Camera::Camera(glm::vec3 startPossition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed , GLfloat startTurnSpeed)
{
	possition = startPossition;
	worldUp	 = startUp; 
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.f);
	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
	update();
}

void Camera::keyControll(bool* keys)
{
	if (keys[GLFW_KEY_W]) {
		possition += front * movementSpeed;
	}
	if (keys[GLFW_KEY_A]) {
		possition -= right * movementSpeed;
	}
	if (keys[GLFW_KEY_S]) {
		possition -= front * movementSpeed;
	}
	if (keys[GLFW_KEY_D]) {
		possition += right * movementSpeed;
	}
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(possition , possition+ front, up);

}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;
	yaw += xChange;
	pitch += yChange;
	if (pitch >= 89.0f) {
		pitch = 89.0f;
	}
	if (pitch <= 89.0f) {
		pitch = 89.0f;
	}
	update();
}

Camera::~Camera()
{
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = cos(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
