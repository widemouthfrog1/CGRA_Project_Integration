#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "Mesh.hpp"

struct treeModel {
	Mesh mesh;
	glm::vec3 position;
};
// geometry
std::vector<treeModel> trees;
std::vector<std::string> rules;

//Project
char axiom[50] = "F";
int depth = 1;
int oldDepth = 1;
int oldSelectedTree = 0;
int selectedTree = 0;
bool selectAll = true;
char rule[50];
char guirules[500] = "";
int rulesIndex = 0;

void loadTrees(std::vector<glm::vec3> positions);
void treeGUI();