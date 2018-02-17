cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0048 NEW)

project("swanson_os" VERSION "0.1.0" LANGUAGES C CXX)

option(SWANSON_BUILD_DOCS     "Build the documentation with Doxygen." ON)
option(SWANSON_BUILD_SOURCE   "Build the source code." ON)
option(SWANSON_BUILD_EXAMPLES "Build the examples." ON)

include_directories("include")

if (SWANSON_BUILD_SOURCE)
	add_subdirectory("src")
endif (SWANSON_BUILD_SOURCE)

if (SWANSON_BUILD_DOCS)
	add_subdirectory("docs")
endif (SWANSON_BUILD_DOCS)

if (SWANSON_BUILD_EXAMPLES)
	add_subdirectory("examples")
endif (SWANSON_BUILD_EXAMPLES)

enable_testing()

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "An operating system for user space.")
set (CPACK_PACKAGE_VENDOR "Taylor Holberton")
set (CPACK_PACKAGE_VERSION_MAJOR ${swanson_os_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${swanson_os_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${swanson_os_VERSION_PATCH})
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_CONTACT "taylorcholberton@gmail.com")

include(CPack)
