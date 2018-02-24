cmake_minimum_required(VERSION 2.6)

add_library("swanson-fat"
	"bpb.h"
	"bpb.c"
	"cache.h"
	"cache.c"
	"disk.h"
	"disk.c"
	"diskfile.h"
	"diskfile.c"
	"error.h"
	"error.c"
	"filelib.h"
	"filelib.c"
	"format.h"
	"format.c"
	"fs.h"
	"fs.c"
	"source.h"
	"source.c"
	"string.h"
	"string.c"
	"type.h"
	"type.c"
	"fat_misc.h"
	"fat_misc.c"
	"fat_table.h"
	"fat_table.c"
	"fat_write.h"
	"fat_write.c"
	"fat_defs.h"
	"fat_list.h"
	"fat_opts.h"
	"types.h")

function(ADD_FAT_UTILITY UTILITY_NAME)
	add_executable("${UTILITY_NAME}" ${ARGN})
	target_link_libraries("${UTILITY_NAME}" "swanson-fat")
endfunction(ADD_FAT_UTILITY UTILITY_NAME)

add_fat_utility("fat-mkdir" "fat-mkdir.c")
add_fat_utility("fat-mkfs" "fat-mkfs.c")
add_fat_utility("fat-cp" "fat-cp.c")
add_fat_utility("fat-ls" "fat-ls.c")
