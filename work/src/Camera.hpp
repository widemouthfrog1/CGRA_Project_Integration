#ifndef CAMERAHPP
#define CAMERAHPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
        Camera(glm::vec3 currentPosition, glm::vec3 targetDirection, glm::vec3 upDirection);
        void onKeyboard(GLFWwindow *currentWindow);
        void onCursor(GLFWwindow *currentWindow, double xPos, double yPos);
        glm::vec3 getCurrentPosition();
        glm::vec3 getTargetDirection();
        glm::vec3 getUpDirection();
        glm::mat4 getViewMatrix();

        glm::vec3 currentPosition;

        double baseCameraSpeed = 10.0;

    private:

        glm::vec3 targetDirection;
        glm::vec3 upDirection;
        
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        float yawAngle = 0.0f;
        float pitchAngle = -30.0f;
        
};

#endif