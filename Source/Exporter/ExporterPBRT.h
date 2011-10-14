#pragma once
#include <string>
#include "AssimpReader.h"

namespace Exporters
{
	namespace PBRT
	{
		bool Exporter(const AssimpReader& meshReader, const std::string& path)
		{
			// Exporter PBRT
			std::cout << "\n";
			std::cout << "Write PBRT file : " << path << "... \n";
			std::fstream file(path.c_str(), std::fstream::out);
	
			for(unsigned int i = 0; i < meshReader.Meshs.size(); i++)
			{
				std::cout << "... Write Mesh";
				file << "AttributeBegin\n";
				file << "Shape \"trianglemesh\"\n";
				// *** Vertex
				file << "\"point P\" [\n";
				for(unsigned int n = 0; n < meshReader.Meshs[i].Vertex.size(); n++)
				{
					glm::vec4 vertex = meshReader.Meshs[i].Vertex[n];
					file << vertex.x / vertex.w << " " << vertex.y / vertex.w  << " " << vertex.x / vertex.z << "\n";
				}
				file << "]\n";
					 		
				// *** UVs
				if(!meshReader.Meshs[i].UVs.empty())
				{
					file << "\"float uv\" [\n";
					for(unsigned int n = 0; n < meshReader.Meshs[i].UVs.size(); n++)
					{
						glm::vec2 uvcoord = meshReader.Meshs[i].UVs[n];
						file << uvcoord.x << " " << uvcoord.y  << "\n";
					}
					file << "]\n";
				}
					 
				// *** Normales 
				if(!meshReader.Meshs[i].Normals.empty())
				{
					file << "\"normal N\" [\n";
					for(unsigned int n = 0; n < meshReader.Meshs[i].Normals.size(); n++)
					{
						glm::vec3 normal = meshReader.Meshs[i].Normals[n];
						file << normal.x << " " << normal.y  << " " << normal.x << "\n";
					}
					file << "]\n";
				}
					 
				// *** Indices
				file << "\"integer indices\" [\n";
				for(unsigned int n = 0; n < meshReader.Meshs[i].Indices.size(); n+=3)
				{
					file << meshReader.Meshs[i].Indices[n] << " " << meshReader.Meshs[i].Indices[n+1] 
						 << " " << meshReader.Meshs[i].Indices[n+2] << "\n";
				}
				file << "]\n";
					 
				file << "AttributeEnd\n";
			}
			return true;
		}
	}
}