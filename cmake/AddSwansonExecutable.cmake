# This function adds an executable,
# enables compiler warnings, and links
# with Swanson library.

function(ADD_SWANSON_EXECUTABLE EXECUTABLE_NAME)
	add_executable(${EXECUTABLE_NAME} ${ARGN})
	target_link_libraries(${EXECUTABLE_NAME} "swanson")
	target_compile_options(${EXECUTABLE_NAME} PRIVATE ${SWANSON_CFLAGS})
endfunction(ADD_SWANSON_EXECUTABLE EXECUTABLE_NAME)
