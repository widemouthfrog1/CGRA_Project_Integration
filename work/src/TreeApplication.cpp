// project
#include "TreeApplication.hpp"
#include "Turtle.h"

std::vector<treeModel> loadTrees(std::vector<glm::vec3> inputPositions) {

	Turtle turtle(glm::vec3(0,1,0));
		
	turtle.loadRules(rules);
	turtle.draw(turtle.getCommand(axiom, depth));
	Mesh mesh = turtle.createMesh();

	std::vector<treeModel> trees;

	for (int i = 0; i < inputPositions.size(); i++) {
		treeModel model;
		model.mesh = mesh;
		model.position = inputPositions.at(i);
		trees.push_back(model);
	}

    return trees;


}

void addRule(std::string newRule){
    rules.push_back(newRule);
}

bool getSelectAll(){
    return selectAll;
}

int getSelectedTree(){
    return selectedTree;
}

void treeGUI()
{
	ImGui::Checkbox("SelectAll", &selectAll);
	ImGui::SliderInt("SelectedTree", &selectedTree, 0, trees.size() - 1);

	ImGui::InputText("Axiom", axiom, 50);
	if (ImGui::Button("Clear Rules")) {
		rules.clear();
		rulesIndex = 0;
		for (int i = 0; i < 500; i++) {
			guirules[i] = '\0';
		}
	}
	if (ImGui::InputText("Add Rule:", rule, 50, ImGuiInputTextFlags_EnterReturnsTrue)) {
		rules.push_back(std::string(rule));
		for (int i = 0; i < rules.at(rules.size() - 1).size(); i++) {
			guirules[rulesIndex + i + 1] = rules.at(rules.size() - 1).at(i);
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