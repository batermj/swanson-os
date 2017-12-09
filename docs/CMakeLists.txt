cmake_minimum_required(VERSION 2.6)

find_package(Doxygen)

if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
	configure_file("Doxyfile.in" "Doxyfile" @ONLY)
	add_custom_target("docs" ALL COMMAND ${DOXYGEN_EXECUTABLE})
	install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION "share/docs")
endif (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
