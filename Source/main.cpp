// Includes Utils
#include "options.h"
// Includes STD
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdarg.h>
#include <limits.h>
// Includes Code proj
#include "AssimpReader.h"
#include "Exporter/Exporters.h"
#ifdef USE_VISU
#include "Visu.h"
#endif

//////////////////////////////////////////
//////////////////////////////////////////
////////// MAIN //////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////

// doc : http://www.cmcrossroads.com/bradapp/ftp/src/libs/C++/Options.html
static const char * optv[] = {
    "h|help",
    "i:file-input    <path>",
    "o?file-output   <path>",
	"t:type          <exportType>",
#ifdef USE_VISU
	"v|visu",
#endif
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
	std::string exportType = "PBRT";
	bool visu = false;

	opts.ctrls(Options::PARSE_POS);
	while(optchar  = opts(iter, optarg) ) {
		switch (optchar)
		{
			case 'h' :
			  showUsage(opts,std::cout);
			  return 0;
			  break;
			case 't':
				if (optarg == NULL)  errors.push_back("type : need an type");
				else exportType = optarg;
				break;
			case 'i':
				if (optarg == NULL)  errors.push_back("infile : need an file path");
				else inFilePath = optarg;
				break;
			case 'v':
				visu = true;
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

	if(inFilePath == "")
	{
		std::cerr << "[Error] You must specify the input file." << std::endl;
		showUsage(opts,std::cerr);
		return 1;
	}

	if(!visu)
	{
		if(exportType == "")
		{
			std::cerr << "[Error] You must specify the export type." << std::endl;
			showUsage(opts,std::cerr);
			return 1;
		}
	}

	AssimpReader meshReader;
	meshReader.LoadFromFile(inFilePath);

	// Show mesh information
	std::cout << "******** Meshs info ********" << std::endl;
	std::cout << "  * filename : " << inFilePath << std::endl;
	std::cout << "\n";
	for(unsigned int i = 0; i < meshReader.Meshs.size(); i++)
	{
		std::cout << "\n";
		std::cout << "------- Mesh info :" << "\n";
		std::cout << "  * number indices : " << meshReader.Meshs[i].Indices.size() << std::endl;
		std::cout << "  * number vertex : " << meshReader.Meshs[i].Vertex.size() << std::endl;
		std::cout << "  * has normal : " << (meshReader.Meshs[i].Normals.empty() ? "False" : "True") << std::endl;
		std::cout << "  * has color : " << (meshReader.Meshs[i].Colors.empty() ? "False" : "True") << std::endl;
		std::cout << "  * has UV : " << (meshReader.Meshs[i].UVs.empty() ? "False" : "True") << std::endl;
	}

	if(visu)
	{
		Visualizer visuGL(meshReader);
		visuGL.Run();
	}
	else
	{
		Exporters::Exporter(meshReader, outFilePath, exportType);
	}
}


