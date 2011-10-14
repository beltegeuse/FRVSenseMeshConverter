#pragma once

#include <string>
#include <iostream>

#include "AssimpReader.h"
#include "ExporterPBRT.h"

namespace Exporters
{
	enum ExporterType
	{
		PBRT_EXPORTER = 0
	};
	
	bool FindExporterType(const std::string& exporterType, ExporterType& typeExporter)
	{
		if(exporterType == "PBRT")
		{
			typeExporter = PBRT_EXPORTER;
			return true;
		}
	
		return false;
	}
	
	
	bool Exporter(const AssimpReader& meshReader, const std::string& path, const std::string& exporterTypeName)
	{
		ExporterType typeExporter;
		if(!FindExporterType(exporterTypeName,typeExporter))
		{
			std::cout << "Error : Unknow Exporter Type\n";
			return false;
		}
	
		switch(typeExporter)
		{
		case PBRT_EXPORTER:
			return PBRT::Exporter(meshReader, path);
			break;
		}
	
		return true;
	
	}
}