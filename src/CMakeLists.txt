
add_executable(${PROJECT_NAME}
	main.cpp
	game_system/MapSys.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ../GEcm)
target_include_directories(${PROJECT_NAME} PUBLIC ../GLoop)
target_include_directories(${PROJECT_NAME} PUBLIC ../GLoop/tools)
target_include_directories(${PROJECT_NAME} PUBLIC game_system)
target_include_directories(${PROJECT_NAME} PUBLIC graphic_system)
target_include_directories(${PROJECT_NAME} PUBLIC .)

