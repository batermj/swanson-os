cmake_minimum_required(VERSION 2.6)

add_library("swanson-fat32"
	"source.h"
	"source.c"
	"fat_access.h"
	"fat_access.c"
	"fat_cache.h"
	"fat_cache.c"
	"fat_filelib.h"
	"fat_filelib.c"
	"fat_format.h"
	"fat_format.c"
	"fat_misc.h"
	"fat_misc.c"
	"fat_string.h"
	"fat_string.c"
	"fat_table.h"
	"fat_table.c"
	"fat_write.h"
	"fat_write.c"
	"fat_defs.h"
	"fat_list.h"
	"fat_opts.h"
	"fat_types.h")

add_executable("fat32-format" "fat32-format.c")

target_link_libraries("fat32-format" "swanson-fat32")
