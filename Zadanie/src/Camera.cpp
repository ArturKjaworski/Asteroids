#include "pch.h"
#include "Camera.h"

Camera::Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
{
	perspectiveMat = glm::perspective(fov, aspect, zNear, zFar);

	this->pos = pos;
	forward = glm::vec3(0.0, 0.0, -1.0);
	up = glm::vec3(0.0, 1.0, 0.0);
}

Camera::~Camera()
{
}

void Camera::SetBoundary(GLuint w, GLuint h)
{
	boundary.x = w;
	boundary.y = h;

	orthoMat = glm::ortho(0.f, boundary.x, 0.f, boundary.y, -1.f, 1.f);
}

glm::mat4 Camera::GetPerspViewProjection() const
{
	return perspectiveMat * glm::lookAt(pos, glm::vec3(0, 0, 0), up);
}

glm::mat4 Camera::GetOrthoViewProjection() const
{
	return orthoMat;
}