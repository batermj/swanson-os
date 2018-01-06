cmake_minimum_required(VERSION 2.6)

add_library("ramfs"
	"dir.h"
	"dir.c"
	"file.h"
	"file.c"
	"fs.h"
	"fs.c")

add_executable("ramfs-tool" "ramfs-tool.c")

target_link_libraries("ramfs-tool" "ramfs")
