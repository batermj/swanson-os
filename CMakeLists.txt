cmake_minimum_required(VERSION 2.6)

project("Swanson OS" C)

option(SWANSON_DOCUMENTATION "Build the documentation with Doxygen." ON)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
	set (SWANSON_WITH_CLANG ON)
endif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")

if (MSVC)
	# 4820 warnings about added structure padding
	# 4996 warnings about 'unsafe' libc functions
	# 4710 warnings about functions like 'printf'
	#      not being inlined.
	list (APPEND SWANSON_CFLAGS /Wall /WX /wd4820 /wd4996 /wd4710)
endif (MSVC)

if (CMAKE_COMPILER_IS_GNUCC OR SWANSON_WITH_CLANG)
	list (APPEND SWANSON_CFLAGS -Wall -Wextra -Werror -Wfatal-errors)
	list (APPEND SWANSON_CFLAGS -iquote "${CMAKE_CURRENT_SOURCE_DIR}")
endif (CMAKE_COMPILER_IS_GNUCC OR SWANSON_WITH_CLANG)

# Use these functions for adding targets
# in this build script.
include("cmake/AddSwansonLibrary.cmake")
include("cmake/AddSwansonExecutable.cmake")
include("cmake/AddSwansonTest.cmake")

add_swanson_library("swanson"
	"crc32.h"
	"crc32.c"
	"debug.h"
	"debug.c"
	"disk.h"
	"disk.c"
	"fs/any.h"
	"fs/any.c"
	"fs/ext4.h"
	"fs/ext4.c"
	"fs/host.h"
	"fs/host.c"
	"fs/vfs.h"
	"fs/vfs.c"
	"fdisk.h"
	"fdisk.c"
	"gpt.h"
	"gpt.c"
	"guid.h"
	"guid.c"
	"kernel.h"
	"kernel.c"
	"memmap.h"
	"memmap.c"
	"partition.h"
	"partition.c"
	"stream.h"
	"stream.c"
	"sstream.h"
	"sstream.c")

add_swanson_executable("gpt-tool" "gpt-tool.c")

add_swanson_executable("ext4-tool" "ext4-tool.c")

add_swanson_executable("swanson-pc" "main.c")

add_custom_command(TARGET "swanson-pc"
	POST_BUILD
	COMMAND $<TARGET_FILE:gpt-tool> "--create" "--image-path" "swanson.img" "format"
	COMMAND $<TARGET_FILE:gpt-tool> "--image-path" "swanson.img" "add-partition"
	COMMENT "Generating swanson.img")

install(TARGETS "swanson" "swanson-pc"
	RUNTIME DESTINATION "bin"
	ARCHIVE DESTINATION "lib"
	LIBRARY DESTINATION "lib")

add_swanson_test("swanson-test"
	"test.h"
	"test.c"
	"memmap-test.h"
	"memmap-test.c"
	"gpt-test.h"
	"gpt-test.c")

enable_testing()

find_package(Doxygen)
if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND AND SWANSON_DOCUMENTATION)
	configure_file("Doxyfile.in" "Doxyfile" @ONLY)
	add_custom_target("doc" ALL COMMAND ${DOXYGEN_EXECUTABLE})
	install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION "doc")
endif (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND AND SWANSON_DOCUMENTATION)
