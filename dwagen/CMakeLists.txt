include_directories(${Leptonica_INCLUDE_DIRS})
add_executable(dwagen dwagen.cpp)
target_link_libraries(dwagen ${Leptonica_LIBRARIES})
