IF(WIN32)
	IF(IS_64BIT)
		# 64-bit
		set(ARCH "x64")
	ELSE()
		# 32-bit
		set(ARCH "x86")
	ENDIF()
    message(STATUS "CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
	add_library(sdlmain INTERFACE)
	IF(MSVC)
		add_library(sdl2 SHARED IMPORTED GLOBAL)
		set_target_properties(sdl2
			PROPERTIES
				IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.28.1/lib/${ARCH}/SDL2.dll"
				IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.28.1/lib/${ARCH}/SDL2.lib"
		)
		target_link_libraries(sdlmain INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.28.1/lib/${ARCH}/SDL2main.lib")
	ELSE(MSVC) #MINGW
		add_library(sdl2 INTERFACE)
		target_link_libraries(sdl2 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.28.1/lib/${ARCH}/SDL2.dll")
		target_link_libraries(sdlmain INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.28.1/lib/${ARCH}/libSDL2main.a")
	ENDIF(MSVC)
	set_target_properties(sdl2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.28.1/include")
    set_target_properties(sdlmain PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.28.1/include")

	# add_target_copy_files("${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.28.1/lib/${ARCH}/SDL2.dll")
ENDIF()