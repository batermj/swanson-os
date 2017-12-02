cmake_minimum_required(VERSION 2.6)

project("Swanson OS")

if (MSVC)
	set (SWANSON_CFLAGS /W3 /WX)
endif (MSVC)

if (CMAKE_COMPILER_IS_GNUCC)
	set (SWANSON_CFLAGS -Wall -Wextra -Werror -Wfatal-errors)
endif (CMAKE_COMPILER_IS_GNUCC)

add_library("swanson"
	"crc32.h"
	"crc32.c"
	"debug.h"
	"debug.c"
	"fdisk.h"
	"fdisk.c"
	"gpt.h"
	"gpt.c"
	"kernel.h"
	"kernel.c"
	"memmap.h"
	"memmap.c"
	"stream.h"
	"stream.c"
	"sstream.h"
	"sstream.c"
	"vfs.h"
	"vfs.c")
target_compile_options("swanson" PRIVATE ${SWANSON_CFLAGS})

add_executable("make-image" "make-image.c")
target_link_libraries("make-image" "swanson")

add_executable("swanson-host" "main.c")
set_target_properties("swanson-host" PROPERTIES OUTPUT_NAME "swanson")
target_link_libraries("swanson-host" "swanson")
target_compile_options("swanson-host" PRIVATE ${SWANSON_CFLAGS})
add_custom_command(TARGET "swanson-host"
	POST_BUILD
	COMMAND $<TARGET_FILE:make-image> "--image-path" "swanson.img"
	COMMENT "Generating swanson.img")

install(TARGETS "swanson" "swanson-host"
	RUNTIME DESTINATION "bin"
	ARCHIVE DESTINATION "lib"
	LIBRARY DESTINATION "lib")

function(ADD_UNIT_TEST TEST_NAME)
	add_executable(${TEST_NAME} ${ARGN})
	target_link_libraries(${TEST_NAME} "swanson")
	add_test(${TEST_NAME} ${TEST_NAME})
endfunction(ADD_UNIT_TEST TEST_NAME)

add_unit_test("memmap-test" "memmap-test.c")

enable_testing()

find_package(Doxygen)
if (DOXYGEN_FOUND)
	configure_file("Doxyfile.in" "Doxyfile" @ONLY)
	add_custom_target("doc" ALL COMMAND ${DOXYGEN_EXECUTABLE})
	install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION "doc")

endif (DOXYGEN_FOUND)
