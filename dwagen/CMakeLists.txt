include_directories(${Leptonica_INCLUDE_DIRS})
add_executable(dwagen dwagen.cpp)
target_link_libraries(dwagen ${Leptonica_LIBRARIES})

install(TARGETS dwagen DESTINATION .)

install(FILES
	morphtemplate1.txt
	morphtemplate2.txt
	hmttemplate1.txt
	hmttemplate2.txt
	DESTINATION .
	)
