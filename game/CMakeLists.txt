set(GAME_SRC IronBarons.cpp
            IronBarons.h
)

add_executable(IronBarons ${EXE_GUI_TYPE} ${GAME_SRC})

target_link_libraries(IronBarons sdlmain)

target_link_libraries(IronBarons engine)

target_include_directories(IronBarons PUBLIC ${CMAKE_SOURCE_DIR}/src/engine)
