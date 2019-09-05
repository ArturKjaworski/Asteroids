#pragma once

class Transform
{
public:
	Transform();
	~Transform();

	void Init(const glm::vec3 & pos, const glm::vec3 & rot, const glm::vec3 & scale);

	//getters
	const glm::mat4 GetModelMat() const;

	const glm::vec3& GetPos() const { return pos; }
	const glm::vec3& GetRot() const{ return rot; }
	const glm::vec3& GetScale() const{ return scale; }

	//setters
	void SetPos(glm::vec3& vec);
	void SetRot(glm::vec3& rot);
	void SetScale(glm::vec3& scale);

private:
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
};

