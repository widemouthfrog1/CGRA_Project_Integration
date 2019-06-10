#include "Camera.hpp"

Camera::Camera(glm::vec3 currentPosition, glm::vec3 targetDirection, glm::vec3 upDirection): 
    currentPosition(currentPosition),
    targetDirection(targetDirection),
    upDirection(upDirection),
    lastFrame(glfwGetTime())
{}

void Camera::onKeyboard(GLFWwindow *currentWindow){

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float cameraSpeed = baseCameraSpeed * deltaTime;

    if(glfwGetKey(currentWindow, GLFW_KEY_W) == GLFW_PRESS) currentPosition += (glm::normalize(glm::vec3(targetDirection.x, 0, targetDirection.z)) * cameraSpeed);
        
    if(glfwGetKey(currentWindow, GLFW_KEY_S) == GLFW_PRESS) currentPosition -= (glm::normalize(glm::vec3(targetDirection.x, 0, targetDirection.z)) * cameraSpeed);

    if(glfwGetKey(currentWindow, GLFW_KEY_A) == GLFW_PRESS){
            
        glm::vec3 leftDirection = glm::cross(targetDirection, upDirection);
        leftDirection = glm::normalize(leftDirection);
        leftDirection *= cameraSpeed;
        currentPosition -= leftDirection;
    }

    if(glfwGetKey(currentWindow, GLFW_KEY_D) == GLFW_PRESS){

        glm::vec3 rightDirection = glm::cross(upDirection, targetDirection);
        rightDirection = glm::normalize(rightDirection);
        rightDirection *= cameraSpeed;
        currentPosition -= rightDirection;
    }

    if(glfwGetKey(currentWindow, GLFW_KEY_SPACE) == GLFW_PRESS) currentPosition += (glm::normalize(upDirection) * cameraSpeed);

    if(glfwGetKey(currentWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) currentPosition -= (glm::normalize(upDirection) * cameraSpeed);

}

void Camera::onCursor(GLFWwindow *currentWindow, double xPos, double yPos){
    
    int windowWidth, windowHeight;
    glfwGetWindowSize(currentWindow, &windowWidth, &windowHeight);

    double xPosition, yPosition;
    glfwGetCursorPos(currentWindow, &xPosition, &yPosition);
    glfwSetCursorPos(currentWindow, windowWidth/2, windowHeight/2);

    yawAngle += (xPos - windowWidth/2) * 0.05;
    pitchAngle -= (yPos - windowHeight/2) * 0.05;

    if(pitchAngle >= 89.0f) pitchAngle = 89.0f;
    if(pitchAngle <= -89.0f) pitchAngle = -89.0f;

}

glm::mat4 Camera::getViewMatrix(){

    float xDirection = cos(glm::radians(pitchAngle)) * cos(glm::radians(yawAngle));
    float yDirection = sin(glm::radians(pitchAngle));
    float zDirection = cos(glm::radians(pitchAngle)) * sin(glm::radians(yawAngle));

    targetDirection.x = xDirection;
    targetDirection.y = yDirection;
    targetDirection.z = zDirection;

    targetDirection = glm::normalize(targetDirection);

    glm::mat4 viewMatrix = glm::lookAt(currentPosition, currentPosition + targetDirection, upDirection);

    return viewMatrix;
}
