#include "Camera.h"

Camera::Camera(const glm::vec3& pos, float fov, GLfloat w, GLfloat h, float zNear, float zFar)
{
	this->pos = pos;
	up = glm::vec3(0.0, 1.0, 0.0);

	SetOrthoMat(w, h);
	SetPerspMat(fov, h / w, zNear, zFar);
}

Camera::~Camera()
{
}

void Camera::SetOrthoMat(GLfloat w, GLfloat h)
{
	orthoMat = glm::ortho(0.f, w, 0.f, h);
}

void Camera::SetPerspMat(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	perspectiveMat = glm::perspective(fov, aspect, zNear, zFar);
}

glm::mat4 Camera::GetPerspViewProjection() const
{
	return perspectiveMat * glm::lookAt(pos, glm::vec3(0, 0, 0), up);
}

glm::mat4 Camera::GetOrthoViewProjection() const
{
	return orthoMat;
}