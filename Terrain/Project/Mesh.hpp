#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 vertexPosition;
    glm::vec3 vertexNormal;
};

class Mesh {
    public:

        Mesh(){}

        Mesh(std::vector<Vertex> meshVertices, std::vector<unsigned int> meshIndices){
            this->meshVertices = meshVertices;
            this->meshIndices = meshIndices;
            setupMesh();
        }

        void drawMesh(){
        
            glBindVertexArray(VertexArrayID);
            glDrawElements(GL_TRIANGLES, meshIndices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
    
        }
        
    private:

        std::vector<Vertex> meshVertices;
        std::vector<unsigned int> meshIndices;

        GLuint VertexArrayID, VertexBufferID, ElementBufferID;

        void setupMesh(){

            glGenVertexArrays(1, &VertexArrayID);
            glGenBuffers(1, &VertexBufferID);
            glGenBuffers(1, &ElementBufferID);

            glBindVertexArray(VertexArrayID);
            glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
            glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(Vertex), &meshVertices[0], GL_STATIC_DRAW);  

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices.size() * sizeof(unsigned int), &meshIndices[0], GL_STATIC_DRAW);

            //Vertex positions
            glEnableVertexAttribArray(0);	
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

            //Vertex Normals
            glEnableVertexAttribArray(1);	
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertexNormal));
            
            glBindVertexArray(0);
        }
};