cmake_minimum_required(VERSION 2.6)

project("Swanson OS" C)

option(SWANSON_DOCUMENTATION "Build the documentation with Doxygen." ON)

if (MSVC)
	# 4820 warnings about added structure padding
	# 4996 warnings about 'unsafe' libc functions
	# 4710 warnings about functions like 'printf'
	#      not being inlined.
	set (SWANSON_CFLAGS /Wall /WX /wd4820 /wd4996 /wd4710)
endif (MSVC)

if (CMAKE_COMPILER_IS_GNUCC)
	set (SWANSON_CFLAGS -Wall -Wextra -Werror -Wfatal-errors)
endif (CMAKE_COMPILER_IS_GNUCC)

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
	"sstream.c"
	"vfs.h"
	"vfs.c")

add_swanson_executable("gpt-tool" "gpt-tool.c")

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

# add_unit_test("memmap-test" "memmap-test.c")
add_swanson_test("gpt-test" "gpt-test.c")

enable_testing()

find_package(Doxygen)
if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND AND SWANSON_DOCUMENTATION)
	configure_file("Doxyfile.in" "Doxyfile" @ONLY)
	add_custom_target("doc" ALL COMMAND ${DOXYGEN_EXECUTABLE})
	install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION "doc")
endif (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND AND SWANSON_DOCUMENTATION)
