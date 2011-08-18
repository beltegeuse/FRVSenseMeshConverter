#ifndef ASSIMPREADER_H_
#define ASSIMPREADER_H_

// Includes assimp
#include <assimp.h>
#include <aiScene.h>
// Includes glm
#include "glm/glm.hpp"
// Includes STL
#include <vector>

class AssimpReader {
public:
	/*
	 * Attributes
	 */
	// === Data arrays
	std::vector<unsigned int> Indices;
	std::vector<float> Vertex;
	std::vector<float> Normal;
	std::vector<float> Color;
	// === Know if there is interesting data
	bool hasNormal;
	bool hasColor;
private:
	// === Other
	int m_indiceOffset;
public:
	AssimpReader();
	virtual ~AssimpReader();

	void LoadFromFile(const std::string& path);
private:
	void LoadNode(const aiScene* scene, aiNode* nd, const glm::mat4& parent);
};

#endif /* ASSIMPREADER_H_ */
