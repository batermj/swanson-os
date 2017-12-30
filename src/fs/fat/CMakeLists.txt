cmake_minimum_required(VERSION 2.6)

add_library("swanson-fat"
	"disk.h"
	"disk.c"
	"diskfile.h"
	"diskfile.c"
	"error.h"
	"error.c"
	"filelib.h"
	"filelib.c"
	"source.h"
	"source.c"
	"string.h"
	"string.c"
	"type.h"
	"type.c"
	"fat_access.h"
	"fat_access.c"
	"fat_cache.h"
	"fat_cache.c"
	"fat_format.h"
	"fat_format.c"
	"fat_misc.h"
	"fat_misc.c"
	"fat_table.h"
	"fat_table.c"
	"fat_write.h"
	"fat_write.c"
	"fat_defs.h"
	"fat_list.h"
	"fat_opts.h"
	"fat_types.h")

add_executable("fat-format" "fat-format.c")

target_link_libraries("fat-format" "swanson-fat")
