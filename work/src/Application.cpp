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

#include "TreeApplication.hpp"

glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 1000.0f);

glm::vec3 cameraPosition = glm::vec3(0, 0.0f, 0);
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

// Options for Terrain Generation

int terrainSize = 100;
int amountOfOctaves = 4;
float lacanarityValue = 1.8;
float persistanceValue = 0.5;
float noiseScale = 25;
float heightMultiplier = 20;
float blendingScale = 2;

float flatHeight = 0.2;

// Options for Erosion

float inertiaConstant = 0.1;
float minSedimentCapacity = 0.01;
float sedimentCapacityFactor = 4.00;
float depositSpeed = 0.01;
float erodeSpeed = 0.01;
float evaporationSpeed = 0.01;
float gravityConstant = 4;

int maxDropletLifetime = 30;
int numIterations = 1000;

int brushSize = 4;

std::vector<std::vector<glm::vec2>> erosionBrushIndices;
std::vector<std::vector<float>> erosionBrushWeights;

GLFWwindow *mainWindow;
Shader simpleShader;

std::vector<treeModel> treeList;

static void glfwErrorCallback(int error, const char* description){
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
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

    rules.push_back("F:F[+CF][-CF][^CF][&CF]");

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

        //groundMesh.drawMesh();

        std::cout << "Size of trees before drawing = " << treeList.size() << std::endl;

        for(unsigned int i = 0; i < treeList.size(); i++){

            //treeShader.useShader();

            //glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 10));
            glm::mat4 newModelViewMatrix = glm::mat4(1.0); //mainCamera.getViewMatrix() * glm::translate(glm::mat4(1.0f), treeList[i].position) * glm::translate(glm::mat4(1.0f), glm::vec3(-(terrainSize-1)/2, 0, -(terrainSize-1)/2)) * scaleMatrix;
            glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, &newModelViewMatrix[0][0]);

            GLint isHighlighted = selectedTree;
            GLuint waterMeshID = glGetUniformLocation(treeShader.getID(), "uTreeSelected");
            glUniform1iv(waterMeshID, 1, &isHighlighted);

            treeList[i].mesh.drawMesh();

            std::cout << treeList[i].mesh.meshVertices[0].vertexPosition.y << std::endl;
            std::cout << treeList[i].mesh.meshVertices[1].vertexPosition.y << std::endl << std::endl;
        }

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

void depositSediment(int xPos, int zPos, float xOffset, float zOffset, float amountOfSediment){

    depthMap[xPos][zPos] += amountOfSediment * (1 - xOffset) * (1 - zOffset);
    depthMap[xPos][zPos + 1] += amountOfSediment * xOffset * (1 - zOffset);
    depthMap[xPos + 1][zPos] += amountOfSediment * (1 - xOffset) * zOffset;
    depthMap[xPos + 1][zPos + 1] += amountOfSediment * xOffset * zOffset;

}

void initializeBrush(int brushRadius){

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

    float xOffset = xPos - xCoordinate;
    float zOffset = zPos - zCoordinate;

    float heightNW = depthMap[xCoordinate][zCoordinate];
    float heightNE = depthMap[xCoordinate][zCoordinate + 1];
    float heightSW = depthMap[xCoordinate + 1][zCoordinate];
    float heightSE = depthMap[xCoordinate + 1][zCoordinate + 1];

    float xGradient = (heightNE - heightNW) * (1 - zOffset) + (heightSE - heightSW) * zOffset;
    float yGradient = (heightSW - heightNW) * (1 - xOffset) + (heightSE - heightNE) * xOffset;

    float pointHeight = heightNW * (1 - xOffset) * (1 - zOffset) + heightNE * xOffset * (1 - zOffset) + heightSW * (1 - xOffset) * zOffset + heightSE * xOffset * zOffset;

    return glm::vec3(xGradient, yGradient, pointHeight);
}

void erodeTerrain(){

    initializeBrush(brushSize);

    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());
    std::uniform_int_distribution<std::mt19937::result_type> randomDistribution(0, (terrainSize - 1) * 10);

    for(int i = 0; i < numIterations; i++) {
        
        float xPosition = randomDistribution(randomNumberGenerator) * 0.1;
        float zPosition = randomDistribution(randomNumberGenerator) * 0.1;

        float xDirection = 0;
        float zDirection = 0;
        
        float currentSpeed = 10;
        float amountOfWater = 1;
        
        float currentAmountOfSediment = 0;

        for (int currentDropletLife = 0; currentDropletLife < maxDropletLifetime; currentDropletLife++) {
        
            int xFloor = (int) xPosition;
            int yFloor = (int) zPosition;

            glm::vec3 heightAndGradient = calculateGradientAndHeight(xPosition, zPosition);

            xDirection = (xDirection * inertiaConstant - heightAndGradient.x * (1 - inertiaConstant));
            zDirection = (zDirection * inertiaConstant - heightAndGradient.y * (1 - inertiaConstant));
            
            float directionLength = sqrt(xDirection * xDirection + zDirection * zDirection);

            if (directionLength != 0) { xDirection /= directionLength; zDirection /= directionLength; }
                
            xPosition += xDirection;
            zPosition += zDirection;

            if((xDirection == 0 && zDirection == 0) || xPosition < 0 || xPosition >= terrainSize - 1 || zPosition < 0 || zPosition >= terrainSize - 1) break;

            float newHeight = calculateGradientAndHeight(xPosition, zPosition).z;
            float deltaHeight = newHeight - heightAndGradient.z;

            float sedimentCapacity = std::max(-deltaHeight * currentSpeed * amountOfWater * sedimentCapacityFactor, minSedimentCapacity);

            if (currentAmountOfSediment > sedimentCapacity || deltaHeight > 0) {

                float amountToDeposit = (deltaHeight > 0) ? std::min (deltaHeight, currentAmountOfSediment) : (currentAmountOfSediment - sedimentCapacity) * depositSpeed;

                currentAmountOfSediment -= amountToDeposit;

                depositSediment(xFloor, yFloor, xPosition - xFloor, zPosition - yFloor, amountToDeposit);

            }
            else {

                float amountToErode = std::min((sedimentCapacity - currentAmountOfSediment) * erodeSpeed, -deltaHeight);

                for (unsigned int brushPoint = 0; brushPoint < erosionBrushIndices[xFloor * terrainSize + yFloor].size(); brushPoint++) {
                                                
                    glm::vec2 nodeIndex = erosionBrushIndices[xFloor * terrainSize + yFloor][brushPoint];

                    int xPos = (int)nodeIndex.x;
                    int zPos = (int)nodeIndex.y;

                    float weighedErodeAmount = amountToErode * erosionBrushWeights[xFloor * terrainSize + yFloor][brushPoint];

                    float deltaSediment = depthMap[xPos][zPos] < weighedErodeAmount ? depthMap[xPos][zPos] : weighedErodeAmount;

                    depthMap[xPos][zPos] -= deltaSediment;
                    currentAmountOfSediment += deltaSediment;

                }

            }

            if(currentSpeed * currentSpeed + deltaHeight * gravityConstant < 0){ break; }
            else currentSpeed = sqrt(currentSpeed * currentSpeed + deltaHeight * gravityConstant);
            amountOfWater *= (1 - evaporationSpeed);

        }

        generateTerrain(false);
        redrawScene();

    }

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

    int numberOfTrees = 10;

    for(unsigned int k = 0; k < numberOfTrees; k++){

        float lowestHeight = 0;
        float heighestHeight = 0.2;
        float minInclineAngle = 0;
        float maxInclineAngle = 90;

        for(glm::vec2 currentPosition: possiblePositions){
            
            int xPos = currentPosition.x;
            int zPos = currentPosition.y;

            glm::vec3 gradientAndHeight = calculateGradientAndHeight(xPos, zPos);

            float currentHeight = gradientAndHeight.z;
            float currentIncline = sqrt(gradientAndHeight.x * gradientAndHeight.x + gradientAndHeight.y * gradientAndHeight.y);

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

    treeList = loadTrees(treePositions);

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

    ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_Once);

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
        if(amountOfOctaves > 8) amountOfOctaves = 8;
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

    ImGui::Separator();

    if(
        ImGui::InputFloat("Inertia", &inertiaConstant, 0.1)   ||
        ImGui::InputFloat("Minimum Sediment Capacity", &minSedimentCapacity, 0.1) ||
        ImGui::InputFloat("Sediment Capacity Factor", &sedimentCapacityFactor, 1) ||
        ImGui::InputFloat("Deposition Scale", &depositSpeed, 0.1) ||
        ImGui::InputFloat("Erode Scale", &erodeSpeed) ||
        ImGui::InputFloat("Evaporation Speed", &evaporationSpeed, 0.5) ||
        ImGui::InputFloat("Gravity", &gravityConstant, 0.5) ||
        ImGui::InputInt("Droplet Lifetime", &maxDropletLifetime) ||
        ImGui::InputInt("Number of Droplets", &numIterations)
    ){

        if(inertiaConstant < 0) lacanarityValue = 0;
        if(inertiaConstant > 1) lacanarityValue = 1;

    }

    if(ImGui::Button("Start Erosion")){
        erodeTerrain();
    }

    treeGUI();

    ImGui::End();

}