
// project
#include "TreeApplication.hpp"
#include "Turtle.h"

std::vector<treeModel> loadTrees(std::vector<glm::vec3> positions) {
	
	Turtle turtle(glm::vec3(0,1,0));
	turtle.loadRules(rules);
	turtle.draw(turtle.getCommand(axiom, depth));
	Mesh mesh = turtle.createMesh();
	for (int i = 0; i < positions.size(); i++) {
		treeModel model;
		model.mesh = mesh;
		model.position = positions.at(i);
		trees.push_back(model);
	}	
		
	return trees;

}

void treeGUI()
{
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
		Turtle turtle(glm::vec3(0, 1, 0));
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