#pragma once

#include "TreeFactory.h"
#include "Turtle.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Turtle;
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


void testFunction(){
    std::cout << "Testing" << std::endl;
}

void loadTrees(std::vector<glm::vec3> positions){
	Turtle turtle(vec3(0,1,0));
	turtle.loadRules(rules);
	turtle.draw(turtle.getCommand(axiom, depth));
	Mesh mesh = turtle.createMesh();
	for (int i = 0; i < positions.size(); i++) {
		treeModel model;
		model.mesh = mesh;
		model.position = positions.at(i);
		trees.push_back(model);
	}
}
void treeGUI() {
	ImGui::Checkbox("SelectAll", &selectAll);
	if (!selectAll) {
		ImGui::SliderInt("SelectedTree", &selectedTree, 0, trees.size() - 1);
	}
	ImGui::InputText("Axiom", axiom, 50);
	if (ImGui::Button("Clear Rules")) {
		rules.clear();
		for (int i = 0; i < 500; i++) {
			guirules[i] = '\0';
		}
	}
	if (ImGui::InputText("Add Rule:", rule, 50, ImGuiInputTextFlags_EnterReturnsTrue)) {
		rules.push_back(std::string(rule));
		for (int i = 0; i < rules.at(rules.size() - 1).size(); i++) {
			guirules[rulesIndex + i];
		}
		rulesIndex += rules.at(rules.size() - 1).size();
	}
	ImGui::Text(guirules);
	ImGui::SliderInt("Depth", &depth, 1, 6);
	if (ImGui::Button("Generate Tree")) {
		Turtle turtle(vec3(0, 1, 0));
		turtle.loadRules(rules);
		turtle.draw(turtle.getCommand(axiom, depth));
		Mesh mesh = turtle.createMesh();
		if (selectAll) {
			for (int i = 0; i < trees.size(); i++) {
				treeModel model;
				model.mesh = mesh;
				model.position = trees.at(i).position;
				trees.at(i) = model;
			}
		}
		else {
			treeModel model;
			model.mesh = mesh;
			model.position = trees.at(selectedTree).position;
			trees.at(selectedTree) = model;
		}
	}

}