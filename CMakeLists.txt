cmake_minimum_required(VERSION 2.6)

project("swanson")

if (CMAKE_COMPILER_IS_GNUCC)
	set (SWANSON_CFLAGS -Wall -Wextra -Werror -Wfatal-errors -ansi -pedantic)
endif (CMAKE_COMPILER_IS_GNUCC)

add_executable("swanson"
	"main.c"
	"debug.h"
	"debug.c"
	"kernel.h"
	"kernel.c"
	"memmap.h"
	"memmap.c"
	"vfs.h"
	"vfs.c")

target_compile_options("swanson" PRIVATE ${SWANSON_CFLAGS})

function(ADD_UNIT_TEST TEST_NAME)
	add_executable(${TEST_NAME} ${ARGN})
	add_test(${TEST_NAME} ${TEST_NAME})
endfunction(ADD_UNIT_TEST TEST_NAME)

add_unit_test("memmap-test"
	"memmap.h"
	"memmap.c"
	"memmap-test.c")

enable_testing()
