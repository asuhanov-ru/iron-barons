cmake_minimum_required(VERSION 3.4)
include(cmake/util.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(Iron_Barones LANGUAGES C CXX)

add_subdirectory(engine)
add_subdirectory(game)

