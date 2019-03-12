if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree built. Please create a builddirectory outside of the source code and call cmake from there")
endif ()

cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -g3 -std=c++17")
set(CXX_STANDARD 17)

project(Bomber VERSION 1.0)

#add_subdirectory(src)

# target_link_libraries(${PROJECT_NAME}
# 	libSrcs
# 	${CMAKE_DL_LIBS}
# 	sfml-graphics
# 	sfml-window
# 	sfml-system
# 	sfml-network
# 	sfml-audio
# )

set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})
