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

	Rule(std::string input,std::string output);
	~Rule();

	std::string applyRule(std::string command);
};