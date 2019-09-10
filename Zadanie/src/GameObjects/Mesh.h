#pragma once

#include "../ObjectLoader.h"

#include <unordered_map>

class Mesh
{
public:
	
	Mesh(const std::string& fileName);
	Mesh(std::vector<glm::vec3>& positions, std::vector<unsigned int>& indices, std::vector<glm::vec2>& texCoords);
	~Mesh();

	void Init();

	void Bind() const;
	void Unbind() const;

	void UpdateTexCoords(const std::vector<glm::vec2>& texCoords);

	GLuint GetIndicesCount() const { return indicesCount; }
	bool IsModelLoaded() { return !model.positions.empty(); }

private:
	enum EVertexBufferType
	{
		POSITION = 0,
		INDEX,
		TEX_COORDS,
		COUNT
	};

	IndexedModel model;

	GLuint indicesCount;
	GLuint vaArrObject;
	GLuint vaBuffers[EVertexBufferType::COUNT];
};



class MeshManager
{
public :
	MeshManager();
	~MeshManager();

	static MeshManager& GetInstance();
	
	void Update();

	void LoadModels();

	void Bind(const std::string& modelPath);
	void Unbind();

	bool IsLoaded();

	const Mesh& GetMesh(const std::string& modelPath) { return *meshes[meshID[modelPath]]; }
	void AddOffsetToUV(float offset);
	std::vector<glm::vec2>& GetBGTexCoords() { return bgTexCoords; }

private:

	std::vector<glm::vec3> bgPositions=
	{
		{-60.0, -0.1f,  75 },
		{-60.0, -0.1f, -75 },
		{ 60.0, -0.1f, -75 },
		{ 60.0, -0.1f,  75 }
	};

	std::vector<glm::vec3> exploPositions =
	{
		{-2.0f, 0.1f,  2.0f },
		{-2.0f, 0.1f, -2.0f },
		{ 2.0f, 0.1f, -2.0f },
		{ 2.0f, 0.1f,  2.0f }
	};

	std::vector<unsigned int> bgIndices =
	{
		0, 1, 2,
		0, 2, 3
	};

	std::vector<glm::vec2> bgTexCoords =
	{
		{ 0.0, 0.0 },
		{ 0.0, 1.0 },
		{ 1.0, 1.0 },
		{ 1.0, 0.0 }
	};

	std::unordered_map<std::string, unsigned int> meshID =
	{
		std::make_pair("res/models/asteroid1.obj" , 0),
		std::make_pair("res/models/player.obj" , 1),
		std::make_pair("res/models/projectile.obj", 2),
		std::make_pair("BG", 3),
		std::make_pair("explo", 4)
	};

	std::vector<Mesh*> meshes;

	unsigned int loadedMeshCounter = 0;
};