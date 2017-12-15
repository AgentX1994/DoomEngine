#include "input.hpp"

#define GET_KEY(var, key) var = glfwGetKey(window, key) == GLFW_PRESS
#define GET_TWO_KEYS(var, key1, key2) var = (glfwGetKey(window, key1) == GLFW_PRESS) | (glfwGetKey(window, key2) == GLFW_PRESS)

void input::pollInput()
{
    GET_TWO_KEYS(moveForwardPressed, GLFW_KEY_W, GLFW_KEY_UP);
    GET_TWO_KEYS(moveBackwardPressed, GLFW_KEY_S, GLFW_KEY_DOWN);
    GET_KEY(moveLeftPressed, GLFW_KEY_A);
    GET_KEY(moveRightPressed, GLFW_KEY_D);

    GET_TWO_KEYS(turnLeftPressed, GLFW_KEY_Q, GLFW_KEY_LEFT);
    GET_TWO_KEYS(turnRightPressed, GLFW_KEY_E, GLFW_KEY_RIGHT);

    GET_KEY(exitPressed, GLFW_KEY_ESCAPE);
}
