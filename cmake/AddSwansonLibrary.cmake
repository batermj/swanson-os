# This function adds library and
# adds compiler warnings.

function(ADD_SWANSON_LIBRARY LIBRARY_NAME)
	add_library(${LIBRARY_NAME} ${ARGN})
	target_compile_options(${LIBRARY_NAME} PRIVATE ${SWANSON_CFLAGS})
endfunction(ADD_SWANSON_LIBRARY LIBRARY_NAME)
