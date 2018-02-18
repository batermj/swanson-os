cmake_minimum_required(VERSION 2.6)

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_swanson_executable("sandbox" "sandbox.cpp")
