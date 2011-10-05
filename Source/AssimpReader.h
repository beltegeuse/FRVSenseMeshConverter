#ifndef ASSIMPREADER_H_
#define ASSIMPREADER_H_

// Includes assimp
#include <assimp.h>
#include <aiScene.h>
// Includes glm
#include "glm/glm.hpp"
// Includes STL
#include <vector>

struct AssimpMesh
{
	std::vector<unsigned int> Indices;
	std::vector<glm::vec4> Vertex;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec3> Colors;
	std::vector<glm::vec2> UVs;
};

class AssimpReader {
public:
	/*
	 * Attributes
	 */
	// === Data arrays
	std::vector<AssimpMesh> Meshs;

public:
	AssimpReader();
	virtual ~AssimpReader();

	void LoadFromFile(const std::string& path);
private:
	void LoadNode(const aiScene* scene, aiNode* nd, const glm::mat4& parent);
};

#endif /* ASSIMPREADER_H_ */
