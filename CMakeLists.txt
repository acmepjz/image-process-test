project(ImageProcessTest)

cmake_minimum_required(VERSION 2.6)

Set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(FreeImage)
find_package(Leptonica)
find_package(FFTW)

if(FreeImage_FOUND AND FFTW_FOUND)
	add_subdirectory(deblur)
endif()

if(Leptonica_FOUND)
	add_subdirectory(dwagen)
endif()

if(FreeImage_FOUND AND Leptonica_FOUND)
	add_subdirectory(despeckle)
endif()
