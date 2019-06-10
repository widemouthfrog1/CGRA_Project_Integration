cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

project(Project)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Application)

include_directories("${PROJECT_SOURCE_DIR}/ImGui")
link_directories("${PROJECT_SOURCE_DIR}/ImGui")
add_subdirectory(ImGui)

SET(Sources
    
	"Application.cpp"
	"Camera.hpp"
	"Camera.cpp"
	"Mesh.hpp"
	"Shader.hpp"
	"Shader.cpp"
    "NoiseMapGenerator.hpp"
)

add_executable(${PROJECT_NAME} ${Sources})

target_link_libraries(${PROJECT_NAME} PRIVATE ImGui)

target_compile_options(${PROJECT_NAME} PUBLIC -g -Wall)
target_link_options(${PROJECT_NAME} PUBLIC -lGL -lGLEW -lglfw)