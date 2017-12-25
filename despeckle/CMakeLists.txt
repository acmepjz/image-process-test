include_directories(${FreeImage_INCLUDE_DIRS} ${Leptonica_INCLUDE_DIRS})
add_executable(despeckle 1.cpp)
target_link_libraries(despeckle ${FreeImage_LIBRARIES} ${Leptonica_LIBRARIES})
