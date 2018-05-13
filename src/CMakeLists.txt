cmake_minimum_required(VERSION 2.6)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

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
	"${INCDIR}/disk.hpp"
	"${SRCDIR}/disk.cpp"
	"${INCDIR}/elf.hpp"
	"${SRCDIR}/elf.cpp"
	"${INCDIR}/hostfs.hpp"
	"${SRCDIR}/hostfs.cpp"
	"fd.h"
	"fd.c"
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
	"${SRCDIR}/thread.cpp"
	"${INCDIR}/tmpfs.hpp"
	"${SRCDIR}/tmpfs.cpp")

target_link_libraries("swanson" "stdc++fs")

set (TOOLKIT_VER "0.0.6")
set (TOOLKIT_URL "https://github.com/swanson-os/swanson-tk/releases/download/v${TOOLKIT_VER}/swanson-tk-${TOOLKIT_VER}.tar.gz")
set (TOOLKIT_OUT "${CMAKE_CURRENT_BINARY_DIR}/swanson-tk-${TOOLKIT_VER}.tar.gz")

set (INIT_PATH "${CMAKE_CURRENT_BINARY_DIR}/swanson-tk-${TOOLKIT_VER}/bin/init")

file (DOWNLOAD ${TOOLKIT_URL} ${TOOLKIT_OUT})

add_custom_command(OUTPUT "${INIT_PATH}"
	COMMAND "${CMAKE_COMMAND}" -E tar xvf "${TOOLKIT_OUT}")

add_custom_command(OUTPUT
		"${CMAKE_CURRENT_BINARY_DIR}/initramfs-data.h"
		"${CMAKE_CURRENT_BINARY_DIR}/initramfs-data.c"
	DEPENDS "rc" "${INIT_PATH}"
	COMMAND $<TARGET_FILE:ramfs-tool> --disk "${CMAKE_CURRENT_BINARY_DIR}/initramfs.img" --create format
	COMMAND $<TARGET_FILE:ramfs-tool> --disk "${CMAKE_CURRENT_BINARY_DIR}/initramfs.img" cp "${INIT_PATH}" "init"
	COMMAND $<TARGET_FILE:rc> --header "${CMAKE_CURRENT_BINARY_DIR}/initramfs-data.h"
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
	"fs-test.hpp"
	"fs-test.cpp"
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

