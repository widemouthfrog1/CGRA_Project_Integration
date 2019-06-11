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
	void link(Vertex &vertex);
	int getConnection(int index);
	int getIndex(int id);
	bool useConnection(Vertex &vertex);
	void resetConnections();
	int connectionsSize();
private:
	std::vector<int> connections;
	std::vector<bool> used;
};