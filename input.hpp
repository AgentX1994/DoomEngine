#pragma once

#include "main.hpp"

class input
{
    private:
        GLFWwindow *window;
        bool moveForwardPressed;
        bool moveBackwardPressed;
        bool moveLeftPressed;
        bool moveRightPressed;
        bool turnLeftPressed;
        bool turnRightPressed;
        bool exitPressed;
        bool resetPressed;

    public:
        input(GLFWwindow* w) : window(w) {}
        void pollInput();

        inline bool getMoveForward() { return moveForwardPressed; }
        inline bool getMoveBackward() { return moveBackwardPressed; }
        inline bool getMoveLeft() { return moveLeftPressed; }
        inline bool getMoveRight() { return moveRightPressed; }
        inline bool getTurnLeft() { return turnLeftPressed; }
        inline bool getTurnRight() { return turnRightPressed; }
        inline bool getExitPressed() { return exitPressed; }
        inline bool getResetPressed() { return resetPressed; }
};
