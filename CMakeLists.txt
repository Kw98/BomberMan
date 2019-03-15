if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree built. Please create a builddirectory outside of the source code and call cmake from there")
endif ()

cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -g3 -std=c++17")
set(CXX_STANDARD 17)

project(Bomber VERSION 1.0)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeFiles)
include("checkBuildType")
include("getIrrlicht")
include("getIrrKlang")

add_subdirectory(src)
add_subdirectory(GLoop)


message("IRRLICHT_INCLUDE_DIR => ${IRRLICHT_INCLUDE_DIR}")
message("IRRLICHT_LIBRARY_DIR => ${IRRLICHT_LIBRARY_DIR}")
message("IRRKLANG_INCLUDE_DIR => ${IRRKLANG_INCLUDE_DIR}")


target_link_libraries(${PROJECT_NAME}
	libGLoop
 	${CMAKE_DL_LIBS}
 	${IRRLICHT_LIBRARY_DIR}
	${IRRKLANG_LIBRARY_DIR}
)


set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})
