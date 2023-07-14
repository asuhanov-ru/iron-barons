include (util)
include (source_groups.cmake)

add_library(engine STATIC ${source_files})
target_compile_features(engine PUBLIC cxx_std_11)

include_directories(${CMAKE_SOURCE_DIR}/engine)