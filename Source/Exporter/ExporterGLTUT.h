
#include "tinyxml.h"

TiXmlElement* writeAttribute(const std::vector<float>& data, int index, int size)
{
	TiXmlElement* attributeNode = new TiXmlElement("attribute");
	attributeNode->SetAttribute("index",index);
	attributeNode->SetAttribute("type","float");
	attributeNode->SetAttribute("size", size);

	std::stringstream ss;
	for(std::size_t i = 0; i < data.size(); i++)
	{
		ss << data[i] << " ";
	}
	attributeNode->LinkEndChild(new TiXmlText(ss.str()));
	return attributeNode;
}

TiXmlElement* writeVAO(const std::string& name, int nbSources, ...)
{
	TiXmlElement* vaoNode = new TiXmlElement("vao");
	vaoNode->SetAttribute("name", name);

	int attribID;
	va_list vl;
    va_start(vl,nbSources);
    for (int i=0;i<nbSources;i++)
	{
    	attribID=va_arg(vl,int);
    	TiXmlElement * sourceNode =  new TiXmlElement("source");
    	sourceNode->SetAttribute("attrib", attribID);
    	vaoNode->LinkEndChild(sourceNode);
	}
    va_end(vl);

	return vaoNode;
}

TiXmlElement* writeIndices(const std::vector<unsigned int>& data, const std::string& cmd = "triangles")
{
	TiXmlElement* indicesNode = new TiXmlElement("indices");
	indicesNode->SetAttribute("cmd", cmd);

	// Find the best type
	if(data.size() < USHRT_MAX)
		indicesNode->SetAttribute("type", "ushort");
	else
		indicesNode->SetAttribute("type", "uint");

	std::stringstream ss;
	for(std::size_t i = 0; i < data.size(); i++)
		ss << data[i] << " ";

	indicesNode->LinkEndChild(new TiXmlText(ss.str()));
	return indicesNode;
}

//void WritegltutExport()
//{
//	////////////////////////
//	// Write the xml file
//	////////////////////////
//	TiXmlDocument doc(outFilePath);
//
//	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
//	TiXmlElement* meshNode = new TiXmlElement("mesh");
//
//	// --- Write attributes
//	meshNode->LinkEndChild(writeAttribute(meshReader.Vertex, 0, 3));
//	if(meshReader.hasColor)
//		meshNode->LinkEndChild(writeAttribute(meshReader.Color, 1, 3));
//	if(meshReader.hasNormal)
//			meshNode->LinkEndChild(writeAttribute(meshReader.Normal, 2, 3));
//
//	// --- Write vao
//	meshNode->LinkEndChild(writeVAO("flat", 1, 0));
//	if(meshReader.hasColor)
//		meshNode->LinkEndChild(writeVAO("color", 2, 0, 1));
//	if(meshReader.hasNormal)
//		meshNode->LinkEndChild(writeVAO("lit", 2, 0, 2));
//	if(meshReader.hasColor && meshReader.hasNormal)
//		meshNode->LinkEndChild(writeVAO("lit-color", 2, 0, 1, 2));
//
//	// --- Write index buffer
//	meshNode->LinkEndChild(writeIndices(meshReader.Indices));
//
//	// --- Write the XML file
//	doc.LinkEndChild(decl);
//	doc.LinkEndChild(meshNode);
//
//	doc.SaveFile(outFilePath);
//}