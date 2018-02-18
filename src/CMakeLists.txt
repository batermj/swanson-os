cmake_minimum_required(VERSION 2.6)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

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

# Use these functions for adding targets
# in this build script.
include("AddSwansonLibrary")
include("AddSwansonExecutable")
include("AddSwansonTest")

set (SRCDIR "${PROJECT_SOURCE_DIR}/src")
set (INCDIR "${PROJECT_SOURCE_DIR}/include/swanson")

add_swanson_library("swanson"
	"assert.h"
	"assert.c"
	"${INCDIR}/cpu.hpp"
	"${SRCDIR}/cpu.cpp"
	"crc32.h"
	"crc32.c"
	"debug.h"
	"debug.c"
	"disk.h"
	"disk.c"
	"${INCDIR}/elf.hpp"
	"${SRCDIR}/elf.cpp"
	"fd.h"
	"fd.c"
	"fs/any.h"
	"fs/any.c"
	"fs/ext4.h"
	"fs/ext4.c"
	"fs/host.h"
	"fs/host.c"
	"fs/vfs.h"
	"fs/vfs.c"
	"fs/ramfs/dir.h"
	"fs/ramfs/dir.c"
	"fs/ramfs/file.h"
	"fs/ramfs/file.c"
	"fs/ramfs/fs.h"
	"fs/ramfs/fs.c"
	"fdisk.h"
	"fdisk.c"
	"gpt.h"
	"gpt.c"
	"gpt-source.h"
	"gpt-source.c"
	"guid.h"
	"guid.c"
	"${INCDIR}/kernel.hpp"
	"${SRCDIR}/kernel.cpp"
	"${INCDIR}/memory-map.hpp"
	"${SRCDIR}/memory-map.cpp"
	"${INCDIR}/memory-section.hpp"
	"${SRCDIR}/memory-section.cpp"
	"memmap.h"
	"memmap.c"
	"module.h"
	"module.c"
	"null.h"
	"null.c"
	"options.h"
	"options.c"
	"partition.h"
	"partition.c"
	"path.h"
	"path.c"
	"${INCDIR}/process.hpp"
	"${SRCDIR}/process.cpp"
	"rstream.h"
	"rstream.c"
	"${INCDIR}/stream.hpp"
	"${SRCDIR}/stream.cpp"
	"stream.h"
	"stream.c"
	"sstream.h"
	"sstream.c"
	"${INCDIR}/thread.hpp"
	"${SRCDIR}/thread.cpp")

set (INIT_VER "0.0.5")
set (INIT_URL "https://github.com/swanson-os/init/releases/download/v${INIT_VER}/init-${INIT_VER}.tar.gz")
set (INIT_OUT "${CMAKE_CURRENT_BINARY_DIR}/init-${INIT_VER}.tar.gz")
file (DOWNLOAD ${INIT_URL} ${INIT_OUT})

add_custom_command(OUTPUT "init"
	COMMAND "${CMAKE_COMMAND}" -E tar xvf "${INIT_OUT}")

add_custom_command(OUTPUT
		"${CMAKE_CURRENT_BINARY_DIR}/initramfs-data.h"
		"${CMAKE_CURRENT_BINARY_DIR}/initramfs-data.c"
	DEPENDS "rc" "init"
	COMMAND
		$<TARGET_FILE:ramfs-tool> --disk "${CMAKE_CURRENT_BINARY_DIR}/initramfs.img" --create format
	COMMAND
		$<TARGET_FILE:ramfs-tool> --disk "${CMAKE_CURRENT_BINARY_DIR}/initramfs.img" cp "init" "init"
	COMMAND
		$<TARGET_FILE:rc> --header "${CMAKE_CURRENT_BINARY_DIR}/initramfs-data.h"
		                  --source "${CMAKE_CURRENT_BINARY_DIR}/initramfs-data.c"
		                  --input "${CMAKE_CURRENT_BINARY_DIR}/initramfs.img"
		                  --name "initramfs_data")

add_swanson_executable("swanson-os" "swanson-os.cpp"
	"${CMAKE_CURRENT_BINARY_DIR}/initramfs-data.h"
	"${CMAKE_CURRENT_BINARY_DIR}/initramfs-data.c")

add_custom_command(TARGET "swanson-os"
	POST_BUILD
	COMMAND $<TARGET_FILE:gpt-tool> "--create" "--image-path" "swanson.img" "format"
	COMMAND $<TARGET_FILE:gpt-tool> "--image-path" "swanson.img" "add-partition"
	COMMENT "Generating swanson.img")

install(TARGETS "swanson" "swanson-os"
	RUNTIME DESTINATION "bin"
	ARCHIVE DESTINATION "lib"
	LIBRARY DESTINATION "lib")

add_swanson_executable("gpt-tool" "gpt-tool.c")

add_swanson_executable("ramfs-tool" "fs/ramfs/ramfs-tool.c")

add_subdirectory("utilities")

add_swanson_test("swanson-test"
	"test.hpp"
	"test.cpp"
	"cpu-test.hpp"
	"cpu-test.cpp"
	"crc32-test.h"
	"crc32-test.c"
	"elf-test.hpp"
	"elf-test.cpp"
	"elf-data.h"
	"elf-data.c"
	"gpt-test.h"
	"gpt-test.c"
	"memory-map-test.hpp"
	"memory-map-test.cpp"
	"memmap-test.h"
	"memmap-test.c"
	"options-test.h"
	"options-test.c"
	"path-test.h"
	"path-test.c")

enable_testing()

