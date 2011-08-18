# Find all *.cpp files
FILE(GLOB MESHCONVERTER_SOURCES *.cpp)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${Assimp_INCLUDE_DIR}
)

# Create an exe
ADD_EXECUTABLE(MeshConverter ${MESHCONVERTER_SOURCES})

# Config link stage
target_link_libraries(
	MeshConverter
	${Assimp_LIBRARY}
)

# Install config
install(TARGETS MeshConverter RUNTIME DESTINATION ${gltutMeshConverter_BINARY_DIR})
	

