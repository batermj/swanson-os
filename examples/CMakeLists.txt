cmake_minimum_required(VERSION 2.6)

function(ADD_SWANSON_EXAMPLE EXAMPLE)
	add_executable(${EXAMPLE} ${ARGN})
	target_link_libraries(${EXAMPLE} "swanson")
endfunction(ADD_SWANSON_EXAMPLE EXAMPLE)

add_swanson_example("sandbox" "sandbox.cpp")
