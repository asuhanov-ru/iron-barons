
SET(BINARY_DESTINATION ".")
SET(LIBRAY_DESTINATION "${BINARY_DESTINATION}")

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(IS_64BIT TRUE)
else()
    set(IS_64BIT FALSE)
endif()
