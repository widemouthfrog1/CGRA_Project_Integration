#pragma once

#include "Vertex.h"
#include "Rule.h"
#include "Branch.h"

#include <vector>
#include <iostream>
#include <string>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include "Mesh.hpp"

struct SavePoint {
	glm::vec3 position;
	glm::vec3 direction;
	float distance;
	float angle;
	float radius;
	std::shared_ptr<Branch> branch;
};


struct Expression {
	std::shared_ptr<Expression> exp1;
	std::shared_ptr<Expression> exp2;
	char op;
	float number;
	int constructor;

	Expression(Expression exp1, char op,  Expression exp2) {
		this->exp1 = std::make_shared<Expression>(exp1);
		this->exp2 = std::make_shared<Expression>(exp2);
		this->op = op;
		constructor = 0;
	}
	Expression(float number) {
		this->number = number;
		constructor = 1;
	}
	Expression() {
		constructor = 2;
	}
	string toString() {
		if (constructor == 0) {
			return "(" +exp1->toString() + op + exp2->toString() + ")";
		}
		else if (constructor == 1) {
			return std::to_string(number);
		}
		else if (constructor == 2) {
			return "NAN";
		}
	}

	float evaluate() {
		if (constructor == 0) {
			if (exp1 != nullptr) {
				if (op == '+') {
					return exp1->evaluate() + exp2->evaluate();
				}
				else if (op == '-') {
					return exp1->evaluate() - exp2->evaluate();
				}
				else if (op == '*') {
					return exp1->evaluate() * exp2->evaluate();
				}
				else if (op == '/') {
					return exp1->evaluate() / exp2->evaluate();
				}
			}
			else {
				return number;
			}
		}
		else if (constructor == 1) {
			return number;
		}
		else if (constructor == 2) {
			return NAN;
		}
		return NAN;
	}

};

class Turtle {
	glm::vec3 position;
	glm::vec3 direction;
	std::vector<SavePoint> stack;
	std::vector<Vertex> vertices;
	float distance = 1;
	float angle = glm::pi<float>()/8;
	float radius = 1;
	std::shared_ptr<Branch> branch;
	bool moved = true;
	int id = 0;
	vector<Rule> ruleList;
	int level = 0;
public:
	Turtle( glm::vec3 startingDirection) {
		this->direction = startingDirection;
	}

	void draw(std::string command) {
		vertices.clear();

		for (int i = 0; i < command.size(); i++) {
			std::string token;
			token.push_back(command.at(i));
			if (token.at(0) == 'F') {
				glm::vec3 startPosition = position;
				this->position += direction * distance;
				Vertex end;
				end.pos = position;

				if (moved) {
					Vertex start;
					start.pos = startPosition;
					start.id = id++;
					end.id = id++;
					end.link(start);
					vertices.push_back(start);
				}
				else {
					end.id = id++;
					end.link(vertices.at(vertices.size() - 1));
				}
				vertices.push_back(end);
				moved = false;
			}
			else if (token.at(0) == 'f') {
				this->position += direction * distance;
				moved = true;
			}
			else if (token.at(0) == '[') {
				SavePoint point;
				point.position = position;
				point.direction = direction;
				point.distance = this->distance;
				point.angle = this->angle;
				this->level++;
				stack.push_back(point);
			}
			else if (token.at(0) == ']') {
				SavePoint point = stack.at(stack.size() - 1);
				stack.pop_back();
				this->position = point.position;
				this->direction = point.direction;
				this->distance = point.distance;
				this->angle = point.angle;
				this->level--;
				moved = true;
			}
			else if (token.at(0) == '+') {
				glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(0, 0, 1));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '-') {
				glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(0, 0, 1));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '&') {//pitch down
				glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(-1, 0, 0));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '^') {//pitch up
				glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(-1, 0, 0));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '\\') {
				glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(0, 1, 0));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '/') {
				glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(0, 1, 0));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '|') {
				glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 0, 1));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == 'D') {
				token = "";
				i += 2;
				int j = 0;
				while (true) {
					if (command.at(i) == ')') {
						if (j == 0) {
							break;
						}
						token += command.at(i);
						i++;
						j--;
					}
					else {
						if (command.at(i) == '(') {
							j++;
						}
						token += command.at(i);
						i++;
					}
				}
				this->distance = parseExpression(token).evaluate();
			}
			else if (token.at(0) == 'A') {
				token = "";
				i += 2;
				int j = 0;
				while (true) {
					if (command.at(i) == ')') {
						if (j == 0) {
							break;
						}
						token += command.at(i);
						i++;
						j--;
					}
					else {
						if (command.at(i) == '(') {
							j++;
						}
						token += command.at(i);
						i++;
					}
				}
				this->angle = parseExpression(token).evaluate();
			}
		}
	}
	Mesh createMesh() {
		for (int i = 0; i < this->vertices.size(); i++) {
			this->vertices.at(i).resetConnections();
		}
		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		for (int i = 0; i < this->vertices.size(); i++) {
			MeshVertex vertex;
			vertex.vertexPosition = this->vertices.at(i).pos;
			vertices.push_back(vertex);
		}
		for (int i = 0; i < this->vertices.size(); i++) {
			Vertex* vertex = &this->vertices.at(i);
			for (int j = 0; j < vertex->connectionsSize(); j++) {
				if (vertex->useConnection(this->vertices.at(vertex->getConnection(j)))) {
					indices.push_back(vertex->id);
					indices.push_back(vertex->getConnection(j));
				}
			}
		}
		Mesh builder = Mesh(vertices, indices);
		return builder;
	}
	std::vector<Branch> createBranches(std::string command, int resolution, glm::mat4 groundAngle) {
		std::vector<Branch> branches;
		for (int i = 0; i < command.size(); i++) {
			std::string token;
			token.push_back(command.at(i));
			if (token.at(0) == 'F') {
				glm::vec3 startPosition = position;
				this->position += direction * distance;
				mat4 rotation = glm::rotate(mat4(1), acos(glm::dot(direction, vec3(0, 1, 0))), glm::cross(direction, vec3(0, 1, 0)));
				Circle end(position, radius, 1, rotation);
				std::shared_ptr<Branch> start = make_shared<Branch>(Branch());
				if (branches.size() == 0) {
					start->base = Circle(startPosition, radius, resolution, groundAngle);
				}
				else if (moved) {
					start = branch;
				}
				else {
					start = make_shared<Branch>(branches.at(branches.size() - 1));
				}
				Branch b = Branch(end);
				start->addBranch(b);
				branches.push_back(b);
				moved = false;
			}
			else if (token.at(0) == 'f') {
				//this would make an odd tree
				this->position += direction * distance;
				moved = true;
			}
			else if (token.at(0) == '[') {
				SavePoint point;
				point.position = position;
				point.direction = direction;
				point.distance = this->distance;
				point.angle = this->angle;
				point.radius = this->radius;
				point.branch = this->branch;
				this->level++;
				stack.push_back(point);
			}
			else if (token.at(0) == ']') {
				SavePoint point = stack.at(stack.size() - 1);
				stack.pop_back();
				this->position = point.position;
				this->direction = point.direction;
				this->distance = point.distance;
				this->angle = point.angle;
				this->radius = point.radius;
				this->branch = point.branch;
				this->level--;
				moved = true;
			}
			else if (token.at(0) == '+') {
				glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(0, 0, 1));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '-') {
				glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(0, 0, 1));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '&') {//pitch down
				glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(-1, 0, 0));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '^') {//pitch up
				glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(-1, 0, 0));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '\\') {
				glm::mat4 rotation = glm::rotate(glm::mat4(1), this->angle, glm::vec3(0, 1, 0));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '/') {
				glm::mat4 rotation = glm::rotate(glm::mat4(1), -this->angle, glm::vec3(0, 1, 0));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == '|') {
				glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 0, 1));
				this->direction = rotation * glm::vec4(direction, 1);
			}
			else if (token.at(0) == 'D') {
				token = "";
				i += 2;
				int j = 0;
				while (true) {
					if (command.at(i) == ')') {
						if (j == 0) {
							break;
						}
						token += command.at(i);
						i++;
						j--;
					}
					else {
						if (command.at(i) == '(') {
							j++;
						}
						token += command.at(i);
						i++;
					}
				}
				this->distance = parseExpression(token).evaluate();
			}
			else if (token.at(0) == 'A') {
				token = "";
				i += 2;
				int j = 0;
				while (true) {
					if (command.at(i) == ')') {
						if (j == 0) {
							break;
						}
						token += command.at(i);
						i++;
						j--;
					}
					else {
						if (command.at(i) == '(') {
							j++;
						}
						token += command.at(i);
						i++;
					}
				}
				this->angle = parseExpression(token).evaluate();
			}
		}
		return branches;
	}
	void loadRules(std::vector<std::string> rules) {
		for (int i = 0; i < rules.size(); i++) {
			std::string input;
			std::string output;
			bool foundColon = false;
			for (char c : rules.at(i)) {
				if (foundColon) {
					output.push_back(c);
				}
				else {
					if (c == ':') {
						foundColon = true;
					}
					else {
						input.push_back(c);
					}
				}
			}
			this->ruleList.push_back(Rule(input, output));
		}
	}
	Expression parseExpression(std::string token) {
		for (int i = 0; i < token.size(); i++) {
			if (token.at(i) == '(') {
				int j = i;
				int d = 1;
				for (i++; d > 0; i++) {
					if (token.at(i) == '(') {
						d++;
					}
					if (token.at(i) == ')') {
						d--;
					}
				}
				i--;
				if (j - i == 1) {
					return Expression(); //empty brackets: ()
				}
				if (i == token.size() - 1) {
					if (token.at(j - 1) == '*') {
						return Expression(parseExpression(token.substr(0, j - 2)), '*', parseExpression(token.substr(j + 1, i - 1)));
					}
					if (token.at(j - 1) == '/') {
						return Expression(parseExpression(token.substr(0, j - 2)), '/', parseExpression(token.substr(j + 1, i - 1)));
					}
					if (token.at(j - 1) == '+') {
						return Expression(parseExpression(token.substr(0, j - 2)), '+', parseExpression(token.substr(j + 1, i - 1)));
					}
					if (token.at(j - 1) == '-') {
						return Expression(parseExpression(token.substr(0, j - 2)), '-', parseExpression(token.substr(j + 1, i - 1)));
					}
				}
				if (token.at(i + 1) == '*') {
					if (j == 0) {
						return Expression(parseExpression(token.substr(j + 1, i - 1)), '*', parseExpression(token.substr(i + 2, token.size() - 1)));
					}
					if (token.at(j - 1) == '*') {
						return Expression(parseExpression(token.substr(0, j - 2)), '*', Expression(parseExpression(token.substr(j + 1, i - 1)), '*', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '/') {
						return Expression(parseExpression(token.substr(0, j - 2)), '/', Expression(parseExpression(token.substr(j + 1, i - 1)), '*', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '+') {
						return Expression(parseExpression(token.substr(0, j - 2)), '+', Expression(parseExpression(token.substr(j + 1, i - 1)), '*', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '-') {
						return Expression(parseExpression(token.substr(0, j - 2)), '-', Expression(parseExpression(token.substr(j + 1, i - 1)), '*', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
				}
				if (token.at(i + 1) == '/') {
					if (j == 0) {
						return Expression(parseExpression(token.substr(j + 1, i - 1)), '/', parseExpression(token.substr(i + 2, token.size() - 1)));
					}
					if (token.at(j - 1) == '*') {
						return Expression(parseExpression(token.substr(0, j - 2)), '*', Expression(parseExpression(token.substr(j + 1, i - 1)), '/', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '/') {
						return Expression(parseExpression(token.substr(0, j - 2)), '/', Expression(parseExpression(token.substr(j + 1, i - 1)), '/', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '+') {
						return Expression(parseExpression(token.substr(0, j - 2)), '+', Expression(parseExpression(token.substr(j + 1, i - 1)), '/', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '-') {
						return Expression(parseExpression(token.substr(0, j - 2)), '-', Expression(parseExpression(token.substr(j + 1, i - 1)), '/', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
				}
				if (token.at(i + 1) == '+') {
					if (j == 0) {
						return Expression(parseExpression(token.substr(j + 1, i - 1)), '+', parseExpression(token.substr(i + 2, token.size() - 1)));
					}
					if (token.at(j - 1) == '*') {
						return Expression(parseExpression(token.substr(0, j - 2)), '*', Expression(parseExpression(token.substr(j + 1, i - 1)), '+', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '/') {
						return Expression(parseExpression(token.substr(0, j - 2)), '/', Expression(parseExpression(token.substr(j + 1, i - 1)), '+', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '+') {
						return Expression(parseExpression(token.substr(0, j - 2)), '+', Expression(parseExpression(token.substr(j + 1, i - 1)), '+', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '-') {
						return Expression(parseExpression(token.substr(0, j - 2)), '-', Expression(parseExpression(token.substr(j + 1, i - 1)), '+', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
				}
				if (token.at(i + 1) == '-') {
					if (j == 0) {
						return Expression(parseExpression(token.substr(j + 1, i - 1)), '-', parseExpression(token.substr(i + 2, token.size() - 1)));
					}
					if (token.at(j - 1) == '*') {
						return Expression(parseExpression(token.substr(0, j - 2)), '*', Expression(parseExpression(token.substr(j + 1, i - 1)), '-', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '/') {
						return Expression(parseExpression(token.substr(0, j - 2)), '/', Expression(parseExpression(token.substr(j + 1, i - 1)), '-', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '+') {
						return Expression(parseExpression(token.substr(0, j - 2)), '+', Expression(parseExpression(token.substr(j + 1, i - 1)), '-', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
					if (token.at(j - 1) == '-') {
						return Expression(parseExpression(token.substr(0, j - 2)), '-', Expression(parseExpression(token.substr(j + 1, i - 1)), '-', parseExpression(token.substr(i + 2, token.size() - 1))));
					}
				}
			}
		}
		for (int i = 0; i < token.size(); i++) {
			if (token.at(i) == '+') {
				return Expression(parseExpression(token.substr(0, i)), '+', parseExpression(token.substr(i + 1, token.size() - i - 1)));
			}
			if (token.at(i) == '-') {
				return Expression(parseExpression(token.substr(0, i)), '-', parseExpression(token.substr(i + 1, token.size() - i - 1)));
			}
		}
		for (int i = 0; i < token.size(); i++) {
			if (token.at(i) == '*') {
				return Expression(parseExpression(token.substr(0, i)), '*', parseExpression(token.substr(i + 1, token.size() - i - 1)));
			}
			if (token.at(i) == '/') {
				return Expression(parseExpression(token.substr(0, i)), '/', parseExpression(token.substr(i + 1, token.size() - i - 1)));
			}
		}
		if (token.at(0) == 'd') {
			return Expression(this->distance);
		}
		if (token.at(0) == 'a') {
			return Expression(this->angle);
		}
		if (token.at(0) == 'l') {
			return Expression(this->level);
		}
		return Expression(std::stof(token, 0));//it must be a float now if correct
	}
	std::string getCommand(std::string axiom, int depth) {
		std::string command = std::string(axiom);
		for (int i = 0; i < depth; i++) {
			for (Rule rule : this->ruleList) {
				command = rule.applyRule(command);
			}
		}
		return command;
	}
};