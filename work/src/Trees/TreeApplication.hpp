#pragma once

#include "TreeFactory.h"
#include "Turtle.h"

class Turtle;

// geometry
std::vector<Mesh> m_models;

//Project
int depth = 1;
int oldDepth = 1;
std::vector<std::string> rules;

void testFunction(){
    std::cout << "Testing" << std::endl;
}

void loadTrees();
void drawTrees();