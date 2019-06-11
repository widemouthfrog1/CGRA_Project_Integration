#pragma once

#include "Letter.h"

#include <vector>
#include <iostream>
#include <string>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Letter;

struct Rule {
	std::string input;
	std::string output;

	Rule(std::string input,std::string output) {
		this->input = input;
		this->output = output;
	}
	~Rule() {}

	std::string applyRule(std::string command) {
		std::string newCommand;
		int j = 0;
		for (int i = 0; i < command.size(); i++) {
			if (command.at(i) == input.at(j) || input.at(j) == '*') {// '*' is the special character that means any character
				j++;
			}
			else {
				for (; j >= 0; j--) {
					newCommand.push_back(command.at(i - j));
				}
				j = 0;
			}
			if (j == input.size()) {
				for (int k = 0; k < output.size(); k++) {
					newCommand.push_back(output.at(k));
				}
				j = 0;
			}
		}
		return newCommand;
	}
};