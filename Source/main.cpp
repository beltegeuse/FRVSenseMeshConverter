// Includes Utils
#include "tinyxml.h"
#include "options.h"
// Includes STD
#include <iostream>
#include <vector>
#include <sstream>
#include <stdarg.h>
#include <limits.h>
// Includes Code proj
#include "AssimpReader.h"

TiXmlElement* writeAttribute(const std::vector<float>& data, int index, int size)
{
	TiXmlElement* attributeNode = new TiXmlElement("attribute");
	attributeNode->SetAttribute("index",index);
	attributeNode->SetAttribute("type","float");
	attributeNode->SetAttribute("size", size);

	std::stringstream ss;
	for(std::size_t i = 0; i < data.size(); i++)
		ss << data[i] << " ";

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

//////////////////////////////////////////
//////////////////////////////////////////
////////// MAIN //////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////

// doc : http://www.cmcrossroads.com/bradapp/ftp/src/libs/C++/Options.html
static const char * optv[] = {
    "h|help",
    "i:file-input    <path>",
    "o|file-output   <path>",
    NULL
} ;

void showUsage(Options& opts, std::ostream& s)
{
	opts.usage(s, "");
}

int main(int argc, char *argv[])
{
	Options  opts(*argv, optv);
	OptArgvIter  iter(--argc, ++argv);
	const char * optarg;
	std::vector<std::string> errors;
	char optchar;

	// variables
	std::string inFilePath;
	std::string outFilePath = "mesh.xml";

	opts.ctrls(Options::PARSE_POS);
	while(optchar  = opts(iter, optarg) ) {
		switch (optchar)
		{
			case 'h' :
			  showUsage(opts,std::cout);
			  return 0;
			  break;
			case 'i':
				if (optarg == NULL)  errors.push_back("infile : need an file path");
				else inFilePath = optarg;
				break;
			case 'o':
				if (optarg == NULL)  errors.push_back("outfile : need an file path");
				else outFilePath = optarg;
				break;
			default :
				errors.push_back("Unknow flag : "+optchar);
				break;
		}
	}

	if(!errors.empty())
	{
		for(size_t i = 0; i < errors.size(); i++)
			std::cerr << "[Error] " << errors[i] << std::endl;
		showUsage(opts,std::cerr);
		return 1;
	}

	AssimpReader meshReader;
	meshReader.LoadFromFile(inFilePath);

	// Show mesh information
	std::cout << "------- Mesh info -------" << std::endl;
	std::cout << "  * filename : " << inFilePath << std::endl;
	std::cout << "  * number indices : " << meshReader.Indices.size() << std::endl;
	std::cout << "  * number vertex : " << meshReader.Vertex.size() << std::endl;
	std::cout << "  * has normal : " << (meshReader.hasNormal ? "True" : "False") << std::endl;
	std::cout << "  * has color : " << (meshReader.hasColor ? "True" : "False") << std::endl;

	////////////////////////
	// Write the xml file
	////////////////////////
	TiXmlDocument doc(outFilePath);

	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	TiXmlElement* meshNode = new TiXmlElement("mesh");

	// --- Write attributes
	meshNode->LinkEndChild(writeAttribute(meshReader.Vertex, 0, 3));
	if(meshReader.hasColor)
		meshNode->LinkEndChild(writeAttribute(meshReader.Color, 1, 3));
	if(meshReader.hasNormal)
			meshNode->LinkEndChild(writeAttribute(meshReader.Normal, 2, 3));

	// --- Write vao
	meshNode->LinkEndChild(writeVAO("flat", 1, 0));
	if(meshReader.hasColor)
		meshNode->LinkEndChild(writeVAO("color", 2, 0, 1));
	if(meshReader.hasNormal)
		meshNode->LinkEndChild(writeVAO("lit", 2, 0, 2));
	if(meshReader.hasColor && meshReader.hasNormal)
		meshNode->LinkEndChild(writeVAO("lit-color", 2, 0, 1, 2));

	// --- Write index buffer
	meshNode->LinkEndChild(writeIndices(meshReader.Indices));

	// --- Write the XML file
	doc.LinkEndChild(decl);
	doc.LinkEndChild(meshNode);

	doc.SaveFile(outFilePath);
}


