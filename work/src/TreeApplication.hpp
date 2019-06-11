//#pragma once
#ifndef TREE
#define TREE

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

static std::vector<treeModel> trees;
static std::vector<std::string> rules;

//Project
static char axiom[50] = "F";
static int depth = 4;
static int oldDepth = 1;
static int oldSelectedTree = 0;
static int selectedTree = 0;
static bool selectAll = true;
static char rule[50];
static char guirules[500] = "";
static int rulesIndex = 0;

std::vector<treeModel> loadTrees(std::vector<glm::vec3> inputPositions, std::vector<std::string> inputRules);
void treeGUI();

#endif