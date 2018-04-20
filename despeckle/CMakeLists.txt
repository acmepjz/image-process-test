add_executable(despeckle
	1.cpp
	hmtgen.101.cpp
	hmtgenlow.101.cpp
	morphgen.102.cpp
	morphgenlow.102.cpp
	)
target_include_directories(despeckle PRIVATE ${FreeImage_INCLUDE_DIRS} ${Leptonica_INCLUDE_DIRS})
target_link_libraries(despeckle ${FreeImage_LIBRARIES} ${Leptonica_LIBRARIES})

install(TARGETS despeckle DESTINATION .)
