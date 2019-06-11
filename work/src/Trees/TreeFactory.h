#pragma once
#include "Variable.h"
#include "Rule.h"
#include "ProjectMath.h"
#include "Circle.h"
#include "Mesh.hpp"

using namespace std;
using namespace glm;

class Variable;
class Rule;

class TreeFactory {
	vector<Variable> alphabet;
public:
	vector<Variable> state;
	TreeFactory(vector<Variable> alphabet, vector<Rule> rules, Variable start);
	~TreeFactory();
	Mesh createTree();
	void next();
};