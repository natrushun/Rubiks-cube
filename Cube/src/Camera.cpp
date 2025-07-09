#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
//#include <math.h>


Camera::Camera()
{
	SetView(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

Camera::~Camera()
{
}

void Camera::SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up)
{
	glm::vec3 prev_eye = eye;
	glm::vec3 prev_at = at;

	eye = _eye;
	at = _at;
	up = _up;

	viewMatrix = glm::lookAt(eye, at, up);
}

void Camera::SetProj(float _angle, float _aspect, float _zNear, float _zFar)
{
	angle = _angle;
	aspect = _aspect;
	zNear = _zNear;
	zFar = _zFar;

	projMatrix = glm::perspective(angle, aspect, zNear, zFar);
}

void Camera::SetAngle(const float _angle)
{
	angle = _angle;
	projMatrix = glm::perspective(angle, aspect, zNear, zFar);
}

void Camera::SetAspect(const float _aspect)
{
	aspect = _aspect;
	projMatrix = glm::perspective(angle, aspect, zNear, zFar);
}

void Camera::SetZNear(const float _zNear)
{
	zNear = _zNear;
	projMatrix = glm::perspective(angle, aspect, zNear, zFar);
}

void Camera::SetZFar(const float _zFar)
{
	zFar = _zFar;
	projMatrix = glm::perspective(angle, aspect, zNear, zFar);
}



static bool firstRightPressCamera = true;
static double lastXCamera = 0.0, lastYCamera = 0.0;

void Camera::Move(TimeInfo* Time, GLFWwindow* window) {

	float speed = 100.0f;
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (firstRightPressCamera)
		{
			lastXCamera = xpos;
			lastYCamera = ypos;
			firstRightPressCamera = false;
		}
		else
		{
			double deltaX = xpos - lastXCamera;
			double deltaY = ypos - lastYCamera;
			lastXCamera = xpos;
			lastYCamera = ypos;
			lastHorizontalAngle = horizontalAngle;
			lastVerticalAngle = verticalAngle;
			horizontalAngle +=  static_cast<float>(deltaX) / 100.0f;
			verticalAngle += static_cast<float>(deltaY) / 100.0f;
		}
	}
	else
	{
		firstRightPressCamera = true;
	}


	if (verticalAngle < -3.00f || verticalAngle>-0.1) verticalAngle = lastVerticalAngle;

	glm::vec3 newEye;
	newEye.y = cosf(verticalAngle) * distance;
	newEye.x = cosf(horizontalAngle) * sinf(verticalAngle) * distance;
	newEye.z = sinf(horizontalAngle) * sinf(verticalAngle) * distance;

	SetView(newEye, at, up);



	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//distance += speed * Time->deltaTime * glm::normalize(at - eye);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//distance -= speed * Time->deltaTime * glm::normalize(at - eye);
	}
	
}