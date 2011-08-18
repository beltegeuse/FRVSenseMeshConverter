#include "AssimpReader.h"
// Includes assimp
#include <assimp/aiPostProcess.h>
// Includes glm
#include "glm/gtc/matrix_inverse.hpp"
// Includes STL
#include <iostream>
#include <vector>
#include <algorithm>

AssimpReader::AssimpReader() :
	m_indiceOffset(0),
	hasNormal(false),
	hasColor(false)
{
}

AssimpReader::~AssimpReader()
{
}

void AssimpReader::LoadFromFile(const std::string& path)
{
	const struct aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_Fast);

	// Read the assimp struct
	LoadNode(scene, scene->mRootNode, glm::mat4());
}

void AssimpReader::LoadNode(const aiScene* scene, aiNode* nd, const glm::mat4& parentMat)
{
	struct aiMatrix4x4 m = nd->mTransformation;
	glm::mat4 localMat(m.a1, m.a2, m.a3, m.a4,
			m.b1, m.b2, m.b3, m.b4,
			m.c1, m.c2, m.c3, m.c4,
			m.d1, m.d2, m.d3, m.d4);
	glm::mat4 worldMat = localMat * parentMat;
	glm::mat3 normalMat = glm::inverseTranspose(glm::mat3(worldMat));
	// draw all meshes assigned to this node
	for (unsigned int n = 0; n < nd->mNumMeshes; ++n)
	{
		/*
		 * Construct the Object
		 */
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		if (mesh->mNumFaces == 0)
		{
			std::cout << "[Info] Skip assimp mesh. No faces. \n";
			continue;
		}

		/***********************************
		 * Read Indice Mesh information
		 ***********************************/
		// Get the indice of all faces
		std::vector<unsigned int> indicesVector;
		std::vector<float> vertexVector;
		unsigned int maxIndice = 0;
		for (unsigned int t = 0; t < mesh->mNumFaces; ++t)
		{
			// Get a face elements
			const struct aiFace* face = &mesh->mFaces[t];
			if (face->mNumIndices != 3)
			{
				std::cout << "[Warning] More or less indices faces (" << face->mNumIndices << "). \n";
				continue;
			}
			// Get all informations
			for (int i = 0; i < face->mNumIndices; i++)
			{
				int index = face->mIndices[i];
				indicesVector.push_back(index);
				maxIndice = std::max(maxIndice, face->mIndices[i]);
			}
		}

		// Append the indice array
		for(unsigned int i = 0; i < indicesVector.size(); i++)
			Indices.push_back(indicesVector[i]+m_indiceOffset);

		// Update offset
		m_indiceOffset+=maxIndice;

		/***********************************
		 * Read Vertex Mesh information
		 ***********************************/
		for(int id = 0; id < maxIndice + 1; id++)
		{
			glm::vec4 vertex(mesh->mVertices[id].x, mesh->mVertices[id].y, mesh->mVertices[id].z,1.f);
			vertex = worldMat*vertex;
			Vertex.push_back(vertex.x/vertex.w);
			Vertex.push_back(vertex.y/vertex.w);
			Vertex.push_back(vertex.z/vertex.w);
		}

		//  * Normal buffer
		if (mesh->HasNormals())
		{
			for(int id = 0; id < maxIndice + 1; id++)
			{
				glm::vec3 normal(mesh->mNormals[id].x,mesh->mNormals[id].y,mesh->mNormals[id].z);
				normal = glm::normalize(normalMat*normal);
				Normal.push_back(normal.x);
				Normal.push_back(normal.y);
				Normal.push_back(normal.z);
			}
			hasNormal = true;
		}
		else
		{
			for(int i = 0; i < (maxIndice + 1)*3; i++)
				Normal.push_back(0.f);
		}

		//   * Color buffer
		if (mesh->GetNumColorChannels() > 0)
		{
			if (mesh->GetNumColorChannels() > 1)
			{
				std::cout << "[Warning] More than one Color channels ( " << mesh->GetNumColorChannels() << " ). \n";
			}

			for(int id = 0; id < maxIndice + 1; id++)
			{
				Color.push_back(mesh->mColors[0][id].r);
				Color.push_back(mesh->mColors[0][id].g);
				Color.push_back(mesh->mColors[0][id].b);
			}
			hasColor = true;
		}
		else
		{
			for(int i = 0; i < (maxIndice + 1)*3; i++)
				Color.push_back(1.f);
		}
	}

	// Add all childrens
	for (int n = 0; n < nd->mNumChildren; ++n)
	{
		LoadNode(scene, nd->mChildren[n], worldMat);
	}
}
