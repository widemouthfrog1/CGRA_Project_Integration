#pragma once
#include "Circle.h"

using namespace std;
using namespace glm;

class Circle;

class Branch {
public:
	Circle base;
	std::vector<Branch> branches;
	Branch() {};
	Branch(Circle base) {
		this->base = base;
	}
	void addBranch(Branch branch) {
		branches.push_back(branch);
	}
};

