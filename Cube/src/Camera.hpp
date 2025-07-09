#pragma once

#include <glm/glm.hpp>
#include "Utils.hpp"

class Camera
{
public:
	Camera();

	

	inline glm::vec3 GetEye() const { return eye; }
	inline glm::vec3 GetAt() const { return at; }
	inline glm::vec3 GetUp() const { return up; }

	inline glm::mat4 GetViewMatrix() const { return viewMatrix; }
	inline glm::mat4 GetProj() const { return projMatrix; }
	inline glm::mat4 GetViewProj() const { return projMatrix * viewMatrix; }


	void SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up);

	inline float GetAngle() const { return angle; }
	void SetAngle(const float _angle);
	inline float GetAspect() const { return aspect; }
	void SetAspect(const float _aspect);
	inline float GetZNear() const { return zNear; }
	void SetZNear(const float _zn);
	inline float GetZFar() const { return zFar; }
	void SetZFar(const float _zf);
	void SetProj(float _angle, float _aspect, float _zn, float _zf);
	void Move(TimeInfo* Time, GLFWwindow* window);

	~Camera();

private:

	// The camera position.
	glm::vec3 eye;
	glm::vec3 up;
	glm::vec3 at;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;


	// projection parameters
	float zNear = 0.01f;
	float zFar = 1000.0f;

	float angle = glm::radians(27.0f);
	float aspect = 1.0f;


	float distance = 20.0f;

	float horizontalAngle = 11.609999f;
	float verticalAngle = -1.100002f;
	float lastHorizontalAngle = 11.609999f;
	float lastVerticalAngle = -1.100002f;
	
};

