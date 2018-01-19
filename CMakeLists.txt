cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0048 NEW)

project("swanson_os" VERSION "0.1.0" LANGUAGES C CXX)

option(SWANSON_BUILD_DOCS "Build the documentation with Doxygen." ON)
option(SWANSON_BUILD_SRC  "Build the source code." ON)

if (SWANSON_BUILD_SRC)
	add_subdirectory("src")
endif (SWANSON_BUILD_SRC)

if (SWANSON_BUILD_DOCS)
	add_subdirectory("docs")
endif (SWANSON_BUILD_DOCS)

enable_testing()
