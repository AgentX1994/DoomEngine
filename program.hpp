#pragma once
#include "main.hpp"

class program
{
    private:
        uint32_t shaderID; // GL ID for this shader program

        // Reads in the given vertex shader and fragment shader source files,
        // compiles them, and attaches them together into a program
        void init(const char* vsFile, const char *fsFile);

        // Default constructor - should never be called
        program();
        // Move constructor - Doesn't exist
        program(const program &s);
    public:
        // Creates a new shader program, using the 2 given source files as the
        // vertex and fragment shaders
        program (const char* vertexFile, const char* fragmentFile);
        // deconstructor, deletes the GL shader program 
        ~program();

        // Use this shader program for rendering
        void use();
        // Stop using this shader program for rendering
        void unuse();
        // Get the GL ID for this shader program
        uint32_t getID();
};
