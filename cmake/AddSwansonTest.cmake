# This function adds an executable,
# enables compiler warnings, links
# with Swanson library and adds it
# to the test list.

include("${CMAKE_CURRENT_LIST_DIR}/AddSwansonExecutable.cmake")

function(ADD_SWANSON_TEST TEST_NAME)
	add_swanson_executable(${TEST_NAME} ${ARGN})
	add_test(${TEST_NAME} ${TEST_NAME})
endfunction(ADD_SWANSON_TEST TEST_NAME)
