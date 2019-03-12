
add_executable(${PROJECT_NAME}
	main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ../GEcm)
target_include_directories(${PROJECT_NAME} PUBLIC ../GLoop)
target_include_directories(${PROJECT_NAME} PUBLIC ../GLoop/tools)

