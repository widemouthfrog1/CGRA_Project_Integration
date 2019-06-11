#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "NoiseMapGenerator.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <random>
#include <algorithm>

//#include "TreeApplication.hpp"

glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::lookAt(glm::vec3(10,10,10), glm::vec3(0,0,0), glm::vec3(0,1,0));
glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 1000.0f);

glm::vec3 cameraPosition = glm::vec3(0, 40.0f, 0);
glm::vec3 cameraDirection = glm::vec3(0, 0, 0);
Camera mainCamera = Camera(cameraPosition, cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f));

void renderTerrainGUI();
void generateTerrain(bool alterHeight);
void generateDepthMap();
void placeTrees();
void processInput(GLFWwindow *currentWindow);
void setupCallbacks(GLFWwindow *currentWindow);

bool toggleOptions = false;

Mesh groundMesh;

std::vector<Mesh> treeMeshes;
std::vector<glm::mat4> treeTransforms;

float** depthMap;
bool** treeMap;

glm::vec2 meshHeightExtremes;

int terrainSize = 100;
int amountOfOctaves = 4;
float lacanarityValue = 1.8;
float persistanceValue = 0.5;
float noiseScale = 25;
float heightMultiplier = 20;
float blendingScale = 2;

float flatHeight = 0.2;

std::vector<std::vector<glm::vec2>> erosionBrushIndices;
std::vector<std::vector<float>> erosionBrushWeights;

GLFWwindow *mainWindow;
Shader simpleShader;

static void glfwErrorCallback(int error, const char* description){
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

Mesh generateFakeTree(){

    std::vector<MeshVertex> vertexPositions;
    std::vector<unsigned int> depthIndices;

    vertexPositions.push_back(MeshVertex{ glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1)});
    vertexPositions.push_back(MeshVertex{ glm::vec3(1, -1, 1), glm::vec3(0, 0, 1)});
    vertexPositions.push_back(MeshVertex{ glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1)});
    vertexPositions.push_back(MeshVertex{ glm::vec3(1, 1, 1), glm::vec3(0, 0, 1)});
    vertexPositions.push_back(MeshVertex{ glm::vec3(-1, 1, -1), glm::vec3(0, 0, 1)});
    vertexPositions.push_back(MeshVertex{ glm::vec3(1, 1, -1), glm::vec3(0, 0, 1)});
    vertexPositions.push_back(MeshVertex{ glm::vec3(-1, -1, -1), glm::vec3(0, 0, 1)});
    vertexPositions.push_back(MeshVertex{ glm::vec3(1, -1, -1), glm::vec3(0, 0, 1)});
    
    depthIndices.push_back(0);
    depthIndices.push_back(1);
    depthIndices.push_back(2);

    depthIndices.push_back(2);
    depthIndices.push_back(1);
    depthIndices.push_back(3);

    depthIndices.push_back(2);
    depthIndices.push_back(3);
    depthIndices.push_back(4);

    depthIndices.push_back(4);
    depthIndices.push_back(3);
    depthIndices.push_back(5);

    depthIndices.push_back(4);
    depthIndices.push_back(5);
    depthIndices.push_back(6);

    depthIndices.push_back(6);
    depthIndices.push_back(5);
    depthIndices.push_back(7);

    depthIndices.push_back(6);
    depthIndices.push_back(7);
    depthIndices.push_back(0);

    depthIndices.push_back(0);
    depthIndices.push_back(7);
    depthIndices.push_back(1);

    depthIndices.push_back(1);
    depthIndices.push_back(7);
    depthIndices.push_back(3);

    depthIndices.push_back(3);
    depthIndices.push_back(7);
    depthIndices.push_back(5);

    depthIndices.push_back(6);
    depthIndices.push_back(0);
    depthIndices.push_back(4);

    depthIndices.push_back(4);
    depthIndices.push_back(0);
    depthIndices.push_back(2);

    return Mesh(vertexPositions, depthIndices);
}

void redrawScene(){

    processInput(mainWindow);

    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    simpleShader.useShader();

    glm::mat4 modelViewMatrix = mainCamera.getViewMatrix() * modelMatrix;

    GLuint modelViewMatrixID = glGetUniformLocation(simpleShader.getID(), "uModelViewMatrix");
    glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, &modelViewMatrix[0][0]);

    GLuint projectionMatrixID = glGetUniformLocation(simpleShader.getID(), "uProjectionMatrix");
    glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);

    GLuint cameraPositionID = glGetUniformLocation(simpleShader.getID(), "uCameraPosition");
    glUniform3fv(cameraPositionID, 1, &mainCamera.currentPosition[0]);

    GLuint heightExtremesID = glGetUniformLocation(simpleShader.getID(), "uHeightExtremes");
    glUniform2fv(heightExtremesID, 1, &meshHeightExtremes[0]);

    GLuint blendingScaleID = glGetUniformLocation(simpleShader.getID(), "uBlendingScale");
    glUniform1fv(blendingScaleID, 1, &blendingScale);

    groundMesh.drawMesh(); 

    for(unsigned int i = 0; i < treeMeshes.size(); i++){

        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 1, 0.1));
        glm::mat4 newModelViewMatrix = mainCamera.getViewMatrix() * treeTransforms[i] * glm::translate(glm::mat4(1.0f), glm::vec3(-(terrainSize-1)/2, 0, -(terrainSize-1)/2)) * scaleMatrix;
        glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, &newModelViewMatrix[0][0]);

        treeMeshes[i].drawMesh();
    }
        
    glfwSwapBuffers(mainWindow);
}

int main(){

    glfwSetErrorCallback(glfwErrorCallback);

    if(!glfwInit()){ 
        std::cout << "Failed to initialise glfw" << std::endl;
        return -1; 
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    mainWindow = glfwCreateWindow(1920, 1080, "CGRA350 Project", NULL, NULL);
    if( mainWindow == NULL ){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glewExperimental = GL_TRUE; 

    glfwMakeContextCurrent(mainWindow); // Initialize GLEW
    glfwSwapInterval(1); // Enable vsync
    if(glewInit() != GLEW_OK) { 
        std::cout << "Failed to initialise glew" << std::endl;
        return -1; 
    }

    glfwSetInputMode(mainWindow, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    generateDepthMap();
    generateTerrain(true);

    simpleShader = Shader(CGRA_SRCDIR + std::string("/src/Shaders/basicVertexShader.glsl"), CGRA_SRCDIR + std::string("/src/Shaders/basicFragmentShader.glsl"));

    Shader treeShader = Shader(CGRA_SRCDIR + std::string("/src/Shaders/basicVertexShader.glsl"), CGRA_SRCDIR + std::string("/src/Shaders/treeFragmentShader.glsl"));

    glClearColor(0.0f, 0.4f, 0.4f, 0.0f);

    //glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one

    setupCallbacks(mainWindow);

    // Setup ImGui //

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
  	ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    const char* glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);  

    // Setup ImGui //

    while(glfwGetKey(mainWindow, GLFW_KEY_Q) != GLFW_PRESS && glfwWindowShouldClose(mainWindow) == 0){
        
        processInput(mainWindow);
        
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderTerrainGUI();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        simpleShader.useShader();

        glm::mat4 modelViewMatrix = mainCamera.getViewMatrix() * modelMatrix;

        GLuint modelViewMatrixID = glGetUniformLocation(simpleShader.getID(), "uModelViewMatrix");
        glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, &modelViewMatrix[0][0]);

        GLuint projectionMatrixID = glGetUniformLocation(simpleShader.getID(), "uProjectionMatrix");
        glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);

        GLuint cameraPositionID = glGetUniformLocation(simpleShader.getID(), "uCameraPosition");
        glUniform3fv(cameraPositionID, 1, &mainCamera.currentPosition[0]);

        GLuint heightExtremesID = glGetUniformLocation(simpleShader.getID(), "uHeightExtremes");
        glUniform2fv(heightExtremesID, 1, &meshHeightExtremes[0]);

        GLuint blendingScaleID = glGetUniformLocation(simpleShader.getID(), "uBlendingScale");
        glUniform1fv(blendingScaleID, 1, &blendingScale);

        groundMesh.drawMesh();

        /*for(unsigned int i = 0; i < trees.size(); i++){

            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 1, 0.1));
            glm::mat4 newModelViewMatrix = mainCamera.getViewMatrix() * glm::translate(glm::mat4(1.0f), trees[i].position) * glm::translate(glm::mat4(1.0f), glm::vec3(-(terrainSize-1)/2, 0, -(terrainSize-1)/2)) * scaleMatrix;
            glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, &newModelViewMatrix[0][0]);

            GLint isHighlighted = selectedTree;
            GLuint waterMeshID = glGetUniformLocation(treeShader.getID(), "uTreeSelected");
            glUniform1iv(waterMeshID, 1, &isHighlighted);

            trees[i].mesh.drawMesh();
        }*/

        if(toggleOptions) ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
        
        glfwSwapBuffers(mainWindow);

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 0;

}

void generateDepthMap(){ 
    
    treeMap = new bool*[terrainSize];
    for(int i = 0; i < terrainSize; ++i) treeMap[i] = new bool[terrainSize];

    generateNoiseMap(terrainSize + 1, noiseScale, lacanarityValue, persistanceValue, amountOfOctaves, &depthMap);

    placeTrees();
}

void generateTerrain(bool alterHeight){

    meshHeightExtremes.x = FLT_MIN;
    meshHeightExtremes.y = FLT_MAX;

    std::vector<MeshVertex> vertexPositions;
    std::vector<unsigned int> depthIndices;

    float centerSubtraction = (terrainSize - 1) / 2.0f;

    for(int i = 0; i < terrainSize; ++i){
        for(int j = 0; j < terrainSize; ++j){
            
            if(alterHeight) depthMap[i][j] = depthMap[i][j] < flatHeight ? depthMap[i][j] : depthMap[i][j] * depthMap[i][j] * heightMultiplier;
            
            float currentHeight = depthMap[i][j];

            if(currentHeight > meshHeightExtremes.x) meshHeightExtremes.x = currentHeight;
            if(currentHeight < meshHeightExtremes.y) meshHeightExtremes.y = currentHeight;

            glm::vec3 vertexPosition = glm::vec3(i - centerSubtraction,  currentHeight, j - centerSubtraction);

            vertexPositions.push_back(MeshVertex{
                vertexPosition, 
                glm::vec3(0, 0, 0)
            });

            if(i < terrainSize - 1 && j < terrainSize - 1){
                
                //First triangle
                depthIndices.push_back(i * terrainSize + j);
                depthIndices.push_back((i + 1) * terrainSize + j + 1);
                depthIndices.push_back(i * terrainSize + j + 1);

                //Second triangle
                depthIndices.push_back(i * terrainSize + j);
                depthIndices.push_back((i + 1) * terrainSize + j);
                depthIndices.push_back((i + 1) * terrainSize + j + 1);
            }

        }
    }

    for(unsigned int currentIndex = 0; currentIndex < depthIndices.size() - 2; currentIndex += 3){

        int vertexIndexA = depthIndices[currentIndex];
        int vertexIndexB = depthIndices[currentIndex + 1];
        int vertexIndexC = depthIndices[currentIndex + 2];

        glm::vec3 vertexA = vertexPositions[vertexIndexA].vertexPosition;
        glm::vec3 vertexB = vertexPositions[vertexIndexB].vertexPosition;
        glm::vec3 vertexC = vertexPositions[vertexIndexC].vertexPosition;

        glm::vec3 sideAB = vertexB - vertexA;
        glm::vec3 sideAC = vertexC - vertexA;

        glm::vec3 vertexNormal = glm::normalize(glm::cross(sideAB, sideAC));

        vertexPositions[vertexIndexA].vertexNormal += vertexNormal;
        vertexPositions[vertexIndexB].vertexNormal += vertexNormal;
        vertexPositions[vertexIndexC].vertexNormal += vertexNormal;

    }

    for(MeshVertex currentVertex: vertexPositions){
        currentVertex.vertexNormal = glm::normalize(currentVertex.vertexNormal);
    }

    groundMesh = Mesh(vertexPositions, depthIndices);
}

void depositSediment(int xPos, int zPos, float cellOffsetX, float cellOffsetZ, float amountOfSediment){

    depthMap[xPos][zPos] += amountOfSediment * (1 - cellOffsetX) * (1 - cellOffsetZ);
    depthMap[xPos][zPos + 1] += amountOfSediment * cellOffsetX * (1 - cellOffsetZ);
    depthMap[xPos + 1][zPos] += amountOfSediment * (1 - cellOffsetX) * cellOffsetZ;
    depthMap[xPos + 1][zPos + 1] += amountOfSediment * cellOffsetX * cellOffsetZ;

}

void initializeBrushIndices(int brushRadius){

    erosionBrushIndices = std::vector<std::vector<glm::vec2>>();
    erosionBrushWeights = std::vector<std::vector<float>>();
    
    int* xOffsets = new int[brushRadius * brushRadius * 4];
    int* yOffsets = new int[brushRadius * brushRadius * 4];
    float* weights = new float[brushRadius * brushRadius * 4];

    float weightSum = 0;
    int addIndex = 0;

    int totalSize = terrainSize * terrainSize;

    for(int i = 0; i < totalSize; i++){
        
        int centreX = i % terrainSize;
        int centreY = i / terrainSize;

        if(centreY <= brushRadius || centreY >= terrainSize - brushRadius || centreX <= brushRadius + 1 || centreX >= terrainSize - brushRadius){
        
            weightSum = 0;
            addIndex = 0;
                
            for (int y = -brushRadius; y <= brushRadius; y++) {
                for (int x = -brushRadius; x <= brushRadius; x++) {
                    
                    float sqrtDistance = x * x + y * y;

                    if(sqrtDistance < brushRadius * brushRadius){

                        int coordX = centreX + x;
                        int coordY = centreY + y;

                        if(coordX >= 0 && coordX < terrainSize && coordY >= 0 && coordY < terrainSize){

                            float weight = 1 - sqrt(sqrtDistance) / brushRadius;
                            
                            weightSum += weight;

                            weights[addIndex] = weight;
                            xOffsets[addIndex] = x;
                            yOffsets[addIndex] = y;

                            addIndex++;
                        }
                    }
                }
            }
        }

        int numEntries = addIndex;
        erosionBrushIndices.push_back(std::vector<glm::vec2>());
        erosionBrushWeights.push_back(std::vector<float>());

        for(int j = 0; j < numEntries; j++){
            
            erosionBrushIndices[i].push_back(glm::vec2(yOffsets[j] + centreY, xOffsets[j] + centreX));
            erosionBrushWeights[i].push_back(weights[j] / weightSum);
        }
    }
}


glm::vec3 calculateGradientAndHeight(float xPos, float zPos){

    int xCoordinate = (int)xPos;
    int zCoordinate = (int)zPos;

    float cellOffsetX = xPos - xCoordinate;
    float cellOffsetZ = zPos - zCoordinate;

    float heightNW = depthMap[xCoordinate][zCoordinate];
    float heightNE = depthMap[xCoordinate][zCoordinate + 1];
    float heightSW = depthMap[xCoordinate + 1][zCoordinate];
    float heightSE = depthMap[xCoordinate + 1][zCoordinate + 1];

    float xGradient = (heightNE - heightNW) * (1 - cellOffsetZ) + (heightSE - heightSW) * cellOffsetZ;
    float yGradient = (heightSW - heightNW) * (1 - cellOffsetX) + (heightSE - heightNE) * cellOffsetX;

    float pointHeight = heightNW * (1 - cellOffsetX) * (1 - cellOffsetZ) + heightNE * cellOffsetX * (1 - cellOffsetZ) + heightSW * (1 - cellOffsetX) * cellOffsetZ + heightSE * cellOffsetX * cellOffsetZ;

    return glm::vec3(xGradient, yGradient, pointHeight);
}

void erodeTerrain(){

    initializeBrushIndices(4);

    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());
    std::uniform_int_distribution<std::mt19937::result_type> randomDistribution(0, (terrainSize - 1) * 10);

    float inertiaConstant = 0.1;
    float minSedimentCapacity = 0.01;
    float sedimentCapacityFactor = 4.00;
    float depositSpeed = 0.01;
    float erodeSpeed = 0.01;
    float evaporationSpeed = 0.01;
    float gravityConstant = 4;

    int maxDropletLifetime = 30;
    
    int numIterations = 1000;

    for(int i = 0; i < numIterations; i++) {
        
        // Create water droplet at random point on map
        float posX = randomDistribution(randomNumberGenerator) * 0.1;
        float posY = randomDistribution(randomNumberGenerator) * 0.1;

        float dirX = 0;
        float dirY = 0;
        
        float speed = 10;
        float water = 1;
        
        float sediment = 0;

        for (int lifetime = 0; lifetime < maxDropletLifetime; lifetime++) {
        
            int nodeX = (int) posX;
            int nodeY = (int) posY;
            
            // Calculate droplet's offset inside the cell (0,0) = at NW node, (1,1) = at SE node
            float cellOffsetX = posX - nodeX;
            float cellOffsetY = posY - nodeY;

            //std::cout <<"Position: " << posX << ", " << posY << std::endl;
            //std::cout <<"Node: " << nodeX << ", " << nodeY << std::endl;
            //std::cout <<"Offset: " << cellOffsetX << ", " << cellOffsetY << std::endl;

            // Calculate droplet's height and direction of flow with bilinear interpolation of surrounding heights
            glm::vec3 heightAndGradient = calculateGradientAndHeight(posX, posY);

            // Update the droplet's direction and position (move position 1 unit regardless of speed)
            dirX = (dirX * inertiaConstant - heightAndGradient.x * (1 - inertiaConstant));
            dirY = (dirY * inertiaConstant - heightAndGradient.y * (1 - inertiaConstant));
            
            // Normalize direction
            float len = sqrt (dirX * dirX + dirY * dirY);

            if (len != 0) {
                dirX /= len;
                dirY /= len;
            }
                
            posX += dirX;
            posY += dirY;

            // Stop simulating droplet if it's not moving or has flowed over edge of map
            if((dirX == 0 && dirY == 0) || posX < 0 || posX >= terrainSize - 1 || posY < 0 || posY >= terrainSize - 1) break;

            // Find the droplet's new height and calculate the deltaHeight
            float newHeight = calculateGradientAndHeight(posX, posY).z;
            float deltaHeight = newHeight - heightAndGradient.z;

            //Calculate the droplet's sediment capacity (higher when moving fast down a slope and contains lots of water)
                
            //std::cout << "Delta: " << -deltaHeight << std::endl;
            //std::cout << "Speed: " << speed << std::endl;

            float sedimentCapacity = std::max(-deltaHeight * speed * water * sedimentCapacityFactor, minSedimentCapacity);

            //std::cout << "Capacity: " << sedimentCapacity << std::endl << std::endl;

            // If carrying more sediment than capacity, or if flowing uphill:
            if (sediment > sedimentCapacity || deltaHeight > 0) {

                    //std::cout << "Capacity: " << sedimentCapacity << std::endl;
                    //std::cout << "Sediment: " << sediment << std::endl;

                    // If moving uphill (deltaHeight > 0) try fill up to the current height, otherwise deposit a fraction of the excess sediment
                    float amountToDeposit = (deltaHeight > 0) ? std::min (deltaHeight, sediment) : (sediment - sedimentCapacity) * depositSpeed;
                    sediment -= amountToDeposit;

                    // Add the sediment to the four nodes of the current cell using bilinear interpolation
                    // Deposition is not distributed over a radius (like erosion) so that it can fill small pits

                    //std::cout << amountToDeposit << std::endl << std::endl;

                    depositSediment(nodeX, nodeY, cellOffsetX, cellOffsetY, amountToDeposit);


                } else {
                    
                    // Erode a fraction of the droplet's current carry capacity.
                    // Clamp the erosion to the change in height so that it doesn't dig a hole in the terrain behind the droplet

                    //std::cout << "Delta: " << -deltaHeight << std::endl;
                    //std::cout << "Sediment: " << sediment << std::endl << std::endl;
                    //std::cout << "Capacity: " << sedimentCapacity << std::endl << std::endl;

                    float amountToErode = std::min((sedimentCapacity - sediment) * erodeSpeed, -deltaHeight);

                    // Use erosion brush to erode from all nodes inside the droplet's erosion radius

                    for (unsigned int brushPointIndex = 0; brushPointIndex < erosionBrushIndices[nodeX * terrainSize + nodeY].size(); brushPointIndex++) {
                                                
                        glm::vec2 nodeIndex = erosionBrushIndices[nodeX * terrainSize + nodeY][brushPointIndex];

                        int xPos = (int)nodeIndex.x;
                        int zPos = (int)nodeIndex.y;

                        //std::cout << amountToErode << std::endl;

                        float weighedErodeAmount = amountToErode * erosionBrushWeights[nodeX * terrainSize + nodeY][brushPointIndex];

                        //std::cout << weighedErodeAmount << std::endl;
                        
                        float deltaSediment = depthMap[xPos][zPos] < weighedErodeAmount ? depthMap[xPos][zPos] : weighedErodeAmount;
                        
                        //std::cout << deltaSediment << std::endl;

                        depthMap[xPos][zPos] -= deltaSediment;
                        sediment += deltaSediment;

                    }

                    //std::cout << std::endl;

                }

                //std::cout << "About to be squared: " << speed * speed + deltaHeight * gravityConstant << std::endl;

                // Update droplet's speed and water content
                if(speed * speed + deltaHeight * gravityConstant < 0){ break; std::cout << "NEGATIVE SQUARE ROOT" << std::endl; }
                else speed = sqrt(speed * speed + deltaHeight * gravityConstant);
                water *= (1 - evaporationSpeed);

            }

            generateTerrain(false);
            redrawScene();

        }

        std::cout << "Erosion complete" << std::endl;
    }

void placeTrees(){

    std::vector<glm::vec2> possiblePositions;

    for(int i = 0; i < terrainSize; i++){
        for(int j = 0; j < terrainSize; j++){
            possiblePositions.push_back(glm::vec2(i, j));        
        }
    }

    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());
    std::shuffle(possiblePositions.begin(), possiblePositions.end(), randomNumberGenerator);

    std::vector<glm::vec3> treePositions;

    int numberOfTrees = 1;

    for(unsigned int k = 0; k < numberOfTrees; k++){

        float lowestHeight = 0;
        float heighestHeight = 0.2;
        float minInclineAngle = 0;
        float maxInclineAngle = 90;

        for(glm::vec2 currentPosition: possiblePositions){
            
            int xPos = currentPosition.x;
            int zPos = currentPosition.y;

            glm::vec3 gradientAndHeight = calculateGradientAndHeight(xPos, zPos);

            float currentHeight = depthMap[xPos][zPos];
            float currentIncline = gradientAndHeight.x; //+ gradientAndHeight.y;

            if(currentHeight >= lowestHeight && currentHeight <= heighestHeight && currentIncline >= minInclineAngle && currentIncline <= maxInclineAngle){
                    
                if(!treeMap[xPos][zPos]){
                        
                    float actualHeight = depthMap[xPos][zPos] < flatHeight ? depthMap[xPos][zPos] : depthMap[xPos][zPos] * depthMap[xPos][zPos] * heightMultiplier;

                    treePositions.push_back(glm::vec3(xPos, actualHeight, zPos));
                    treeMap[xPos][zPos] = true;

                    break;                    
                }
            }
        }
    }

    //loadTrees(treePositions);

}

void handleKeys(GLFWwindow* currentWindow, int keyCode, int scanCode, int keyAction, int modKeys){
    if(keyAction == GLFW_PRESS && keyCode == GLFW_KEY_ESCAPE){
        if(toggleOptions){
            int windowWidth, windowHeight;
            glfwGetWindowSize(currentWindow, &windowWidth, &windowHeight);
            glfwSetCursorPos(currentWindow, windowWidth/2, windowHeight/2);
            glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        toggleOptions = !toggleOptions;
    }
}

void handleCursor(GLFWwindow* currentWindow, double xPos, double yPos){
    if(!toggleOptions) mainCamera.onCursor(currentWindow, xPos, yPos);
}

void handleScroll(GLFWwindow* currentWindow, double xOffset, double yOffset){
    mainCamera.baseCameraSpeed += yOffset * 5.0;
    if(mainCamera.baseCameraSpeed < 10) mainCamera.baseCameraSpeed = 10;
    if(mainCamera.baseCameraSpeed > 100) mainCamera.baseCameraSpeed = 100;
}

void setupCallbacks(GLFWwindow *currentWindow){
    glfwSetKeyCallback(currentWindow, handleKeys);
    glfwSetCursorPosCallback(currentWindow, handleCursor);
    glfwSetScrollCallback(currentWindow, handleScroll);
}

void processInput(GLFWwindow *currentWindow){
    mainCamera.onKeyboard(currentWindow);
}

void renderTerrainGUI(){

	ImGui::Begin("Options");
    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    
    ImGui::InputInt("Size", &terrainSize);

    ImGui::Separator();

    if( 
        ImGui::InputFloat("Lacanarity", &lacanarityValue, 0.1)   ||
        ImGui::InputFloat("Persistance", &persistanceValue, 0.1) ||
        ImGui::InputFloat("Scale", &noiseScale, 1) ||
        ImGui::InputFloat("Height", &heightMultiplier, 0.1) ||
        ImGui::InputInt("Octaves", &amountOfOctaves) ||
        ImGui::InputFloat("Blending", &blendingScale, 0.5)
    ){  
        if(lacanarityValue < 1) lacanarityValue = 1;
        if(persistanceValue < 0) persistanceValue = 0;
        if(persistanceValue > 1) persistanceValue = 1;
        if(noiseScale <= 0) noiseScale = 1;
        if(amountOfOctaves < 0) amountOfOctaves = 0;
        if(heightMultiplier < 0) heightMultiplier = 0;
        if(blendingScale < 1) blendingScale = 1;
        if(blendingScale > 10) blendingScale = 10;

        generateDepthMap();
        generateTerrain(true);
    }

    if(ImGui::Button("Generate Terrain")){
        generateDepthMap();
        generateTerrain(true); 
    }
    if(ImGui::Button("Start Erosion")){
        erodeTerrain();
    }

    ImGui::Separator();

    //treeGUI();

    ImGui::End();

}