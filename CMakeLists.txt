cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0048 NEW)

project("swanson_os" VERSION "0.1.0" LANGUAGES C CXX)

option(SWANSON_BUILD_DOCS     "Build the documentation with Doxygen." ON)
option(SWANSON_BUILD_SOURCE   "Build the source code." ON)
option(SWANSON_BUILD_EXAMPLES "Build the examples." ON)

include_directories("include")

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
	set (SWANSON_WITH_CLANG ON)
endif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")

if (MSVC)
	# 4820 warnings about added structure padding
	# 4996 warnings about 'unsafe' libc functions
	# 4710 warnings about functions like 'printf'
	#      not being inlined.
	list (APPEND SWANSON_CFLAGS /Wall /WX /wd4820 /wd4996 /wd4710)
	lsit (APPEND SWANSON_CFLAGS /DSWANSON_WITH_INITRAMFS_DATA_H=1)
endif (MSVC)

if (CMAKE_COMPILER_IS_GNUCC OR SWANSON_WITH_CLANG)
	set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -std=c++17")
	list (APPEND SWANSON_CFLAGS -Wall -Wextra -Werror -Wfatal-errors)
	list (APPEND SWANSON_CFLAGS -DSWANSON_WITH_INITRAMFS_DATA_H=1)
endif (CMAKE_COMPILER_IS_GNUCC OR SWANSON_WITH_CLANG)

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
