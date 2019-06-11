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
static bool selectAll = false;
static char rule[50];
static char guirules[500] = "\nF:F[+CF][-CF][^CF][&CF]\nCF:D(d/l)F";
static int rulesIndex = 34;

std::vector<treeModel> loadTrees(std::vector<glm::vec3> inputPositions);
bool getSelectAll();
int getSelectedTree();
std::vector<treeModel> getTrees();
void addRule(std::string newRule);
void treeGUI();

#endif