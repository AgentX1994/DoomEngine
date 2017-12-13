#include "input.hpp"

#define GETKEY(var, key) var = glfwGetKey(window, key) == GLFW_PRESS

void input::pollInput()
{
    GETKEY(moveForwardPressed, GLFW_KEY_W);
    GETKEY(moveBackwardPressed, GLFW_KEY_S);
    GETKEY(moveLeftPressed, GLFW_KEY_A);
    GETKEY(moveRightPressed, GLFW_KEY_D);

    GETKEY(turnLeftPressed, GLFW_KEY_Q);
    GETKEY(turnRightPressed, GLFW_KEY_E);

    GETKEY(exitPressed, GLFW_KEY_ESCAPE);
}
