#pragma once
class Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar);
	~Camera();

	void SetBoundary(GLuint w, GLuint h);

	glm::mat4 GetPerspViewProjection() const;
	glm::mat4 GetOrthoViewProjection() const;

private:
	glm::mat4 perspectiveMat;
	glm::mat4 orthoMat;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;

	glm::vec2 boundary;
};

