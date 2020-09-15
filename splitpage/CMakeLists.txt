include_directories(${FreeImage_INCLUDE_DIRS})
add_executable(splitpage 1.cpp)
target_link_libraries(splitpage ${FreeImage_LIBRARIES})

install(TARGETS splitpage DESTINATION .)
