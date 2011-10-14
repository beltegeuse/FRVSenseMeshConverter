# Find all *.cpp files
SET(MESHCONVERTER_SOURCES 
	main.cpp
	options.cpp
	tinystr.cpp
	tinyxml.cpp
	tinyxmlerror.cpp
	tinyxmlparser.cpp
	AssimpReader.cpp
	
	# For VS Visualisation
	AssimpReader.h
	Exporter/ExporterPBRT.h
	Exporter/ExporterGLTUT.h
	Exporter/Exporters.h
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${Assimp_INCLUDE_DIR}
)

# IF need VISU
IF(ENABLE_VISU)
	SUBDIRS(C3)
	ADD_DEFINITIONS(-DUSE_VISU)
	SET(MESHCONVERTER_SOURCES 
		${MESHCONVERTER_SOURCES}
		Visu.h
		Visu.cpp
	)
	INCLUDE_DIRECTORIES(
		${CMAKE_CURRENT_SOURCE_DIR}/C3/Source
	)
ENDIF()

# Create an exe
ADD_EXECUTABLE(MeshConverter ${MESHCONVERTER_SOURCES})

# Config link stage
IF(ENABLE_VISU)
target_link_libraries(
	MeshConverter
	${Assimp_LIBRARY}
	C3
)
ELSE()
target_link_libraries(
	MeshConverter
	${Assimp_LIBRARY}
)
ENDIF()

# Install config
install(TARGETS MeshConverter RUNTIME DESTINATION ${gltutMeshConverter_BINARY_DIR})
	

