#pragma once

#include <vector>
#include <iostream>
#include <string>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct Vertex {

	int id;
	int branch;
	glm::vec3 pos;
	void link(Vertex &vertex) {
		connections.push_back(vertex.id);
		used.push_back(false);
		vertex.connections.push_back(id);
		vertex.used.push_back(false);
	}
	int getConnection(int index) {
		return connections.at(index);
	}
	int getIndex(int id) {
		for (int i = 0; i < connections.size(); i++) {
			if (connections.at(i) == id) {
				return i;
			}
		}
		return -1;
	}
	bool useConnection(Vertex &vertex) {
		if (used.at(getIndex(vertex.id)) == true) {
			return false;
		}
		used.at(getIndex(vertex.id)) = true;
		vertex.used.at(vertex.getIndex(this->id)) = true;
		return true;
	}
	void resetConnections() {
		for (int i = 0; i < used.size(); i++) {
			used.at(i) = false;
		}
	}
	int connectionsSize() {
		return connections.size();
	}
private:
	std::vector<int> connections;
	std::vector<bool> used;
};