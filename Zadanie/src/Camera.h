#pragma once
#include "pch.h"

class Camera
{
public:
	Camera(const glm::vec3& pos, float fov, GLfloat w, GLfloat h, float zNear, float zFar);
	~Camera();

	void SetOrthoMat(GLfloat w, GLfloat h);
	void SetPerspMat(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar);

	glm::mat4 GetPerspViewProjection() const;
	glm::mat4 GetOrthoViewProjection() const;

private:
	glm::mat4 perspectiveMat;
	glm::mat4 orthoMat;
	glm::vec3 pos;
	glm::vec3 up;
};

