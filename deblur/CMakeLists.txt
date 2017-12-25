include_directories(${FreeImage_INCLUDE_DIRS} ${FFTW_INCLUDES})
add_executable(deblur 1.cpp)
target_link_libraries(deblur ${FreeImage_LIBRARIES} ${FFTW_LIBRARIES} )
