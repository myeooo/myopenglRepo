#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
class Camera
{
public: 
	Camera() {}
	Camera(glm::vec3 possition, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	
	void keyControll(bool* key);

	glm::mat4 calculateViewMatrix();
	void mouseControl(GLfloat xChange, GLfloat yChange);
	~Camera();
private: 
	glm::vec3 possition;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;
	 
	void update();

};

