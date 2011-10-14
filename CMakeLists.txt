PROJECT(gltutMeshConverter)

# CMake configuration
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Config/CMake ${CMAKE_MODULE_PATH})
SET (ENABLE_VISU OFF CACHE BOOL "Use VISU command to displat the model")


# Search for dependancies
FIND_PACKAGE(Assimp REQUIRED)

# Go to sub dir
SUBDIRS(Source)
