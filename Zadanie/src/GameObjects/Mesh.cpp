#include "pch.h"

#include "Mesh.h"

#include <vector>
//////////////////////////////////////////////////////////////////////////////
//  								Mesh									//
//////////////////////////////////////////////////////////////////////////////
Mesh::Mesh(const std::string& fileName)
{
//	Init(OBJModel(fileName).ToIndexedModel());

	IndexedModel model = OBJModel(fileName).ToIndexedModel();
	Init(model.positions, model.indices, model.texCoords);
}

Mesh::Mesh(std::vector<glm::vec3>& positions, std::vector<unsigned int>& indices, std::vector<glm::vec2>& texCoords)
{
	Init(positions, indices, texCoords);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vaBuffers[POSITION]);
	glDeleteBuffers(1, &vaBuffers[INDEX]);
	glDeleteBuffers(1, &vaBuffers[TEX_COORDS]);
	glDeleteVertexArrays(1, &vaArrObject);
}

void Mesh::Init(const IndexedModel & model)
{
	indicesCount = model.indices.size();

	glGenVertexArrays(1, &vaArrObject);
	glBindVertexArray(vaArrObject);

	//vertex buffer
	glGenBuffers(EVertexBufferType::COUNT, vaBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, vaBuffers[EVertexBufferType::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	
	glBindBuffer(GL_ARRAY_BUFFER, vaBuffers[EVertexBufferType::TEX_COORDS]);
	glBufferData(GL_ARRAY_BUFFER, model.texCoords.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	

	//indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vaBuffers[EVertexBufferType::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Init(const std::vector<glm::vec3>& positions, const std::vector<unsigned int>& indices, const std::vector<glm::vec2>& texCoords)
{
	indicesCount = indices.size();

	glGenVertexArrays(1, &vaArrObject);
	glBindVertexArray(vaArrObject);

	//vertex buffer
	glGenBuffers(EVertexBufferType::COUNT, vaBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, vaBuffers[EVertexBufferType::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, vaBuffers[EVertexBufferType::TEX_COORDS]);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords[0]), &texCoords[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


	//indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vaBuffers[EVertexBufferType::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Bind() const
{
	glBindVertexArray(vaArrObject);

	//index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vaBuffers[EVertexBufferType::INDEX]);	
}

void Mesh::Unbind() const
{
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::UpdateTexCoords(const std::vector<glm::vec2>& texCoords)
{
	Bind();
	glBindBuffer(GL_ARRAY_BUFFER, vaBuffers[EVertexBufferType::TEX_COORDS]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, texCoords.size() * sizeof(texCoords[0]), &texCoords[0]);
	Unbind();
}

//////////////////////////////////////////////////////////////////////////////
//  							Mesh Manager								//
//////////////////////////////////////////////////////////////////////////////
MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
	for (Mesh* mesh : meshes)
		delete(mesh);
}

MeshManager& MeshManager::GetInstance()
{
	static MeshManager instance;
	return instance;
}

void MeshManager::LoadModels()
{
	for (auto& pair : meshID)
		if (pair.second != meshID.size() - 1)
			meshes.push_back(new Mesh(pair.first));

	meshes.push_back(new Mesh(bgPositions, bgIndices, bgTexCoords));
}

void MeshManager::Bind(const std::string & modelPath)
{
	meshes[meshID[modelPath]]->Bind();
}

void MeshManager::Unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshManager::AddOffsetToUV(float offset)
{
	for (size_t ii = 0; ii < bgTexCoords.size(); ++ ii)
	{
		bgTexCoords[ii][1] += offset;
		if (ii == 0 || ii == 3)
		{
			if (bgTexCoords[ii][1] >= 1)
				bgTexCoords[ii][1] -= int(bgTexCoords[ii][1]);
		}
		else
			if (bgTexCoords[ii][1] >= 2)
				bgTexCoords[ii][1] -= int(bgTexCoords[ii][1] - 1);
	}
	meshes[meshID["BG"]]->UpdateTexCoords(bgTexCoords);
}
