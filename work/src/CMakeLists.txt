
#########################################################
# Source Files
#########################################################

# ----TODO------------------- #
# list your source files here #
# --------------------------- #
SET(sources

	"Application.cpp"
	"Camera.hpp"
	"Camera.cpp"
	"Mesh.hpp"
	"Shader.hpp"
	"Shader.cpp"
    "NoiseMapGenerator.hpp"

	"TreeApplication.cpp"
	"TreeApplication.hpp"
	"ProjectMath.cpp"
	"ProjectMath.h"
	"Rule.h"
	"Rule.cpp"
	"Turtle.cpp"
	"Turtle.h"
	"Vertex.cpp"
	"Vertex.h"

	"CMakeLists.txt"
)

# Add executable target and link libraries
add_executable(${CGRA_PROJECT} ${sources})

# ----TODO--------------------- #
# list your subdirectories here #
# ----------------------------- #


#########################################################
# Link and Build Executable
#########################################################

# Set source groups (helper method)
target_source_group_tree(${CGRA_PROJECT})

# Set working folder
target_compile_definitions(${CGRA_PROJECT} PRIVATE "-DCGRA_SRCDIR=\"${PROJECT_SOURCE_DIR}\"")

# Link usage requirements
target_link_libraries(${CGRA_PROJECT} PRIVATE glew glfw ${GLFW_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE stb imgui)

# For experimental <filesystem>
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_link_libraries(${CGRA_PROJECT} PRIVATE -lstdc++fs)
endif()
