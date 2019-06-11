# Source files
set(sources

        "TreeApplication.cpp"
        "TreeApplication.hpp"
        "Branch.cpp"
        "Branch.h"
        "BranchFactory.cpp"
        "BranchFactory.h"
        "Circle.cpp"
        "Circle.h"
        "Letter.cpp"
        "Letter.h"
        "ProjectMath.cpp"
        "ProjectMath.h"
        "Rule.cpp"
        "Rule.h"
        "TreeFactory.cpp"
        "TreeFactory.h"
        "Turtle.cpp"
        "Turtle.h"
        "Variable.cpp"
        "Variable.h"
        "Vertex.cpp"
        "Vertex.h"

        "CMakeLists.txt"

        )

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})
