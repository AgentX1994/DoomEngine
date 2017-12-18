#pragma once
#include "main.hpp"
#include "program.hpp"

class screen
{
    private:
        static GLfloat vertices[];              // Stores vertices for screen (simple rectangle)
        static GLuint indices[];                // Triangle vertex indices for screen
        static GLfloat uvs[];                   // uvs for screen to determine pixel location
        static std::shared_ptr<program> shader; // Shader to use for drawing

        static uint32_t vaoID;                  // GL Vertex Array Object ID
        static uint32_t vboID;                  // GL Vertex Buffer Object ID
        static uint32_t iboID;                  // GL Element Array Buffer ID (holds indices)
        static uint32_t uvBuffer;               // GL Array Object to hold UVs

        static GLuint distTextureID;            // GL 1D texture holding distances
        static GLuint colorTextureID;           // GL 1D texture holding colors

        static bool initialized;                // has the static class been initialized?
    public:

        // Creates all the neccessary GL objects for this class to function properly
        static void init();

        // Sends the float array data to the distance texture
        static void setDistances(float d[], int n);

        // Sends the color information to the color texture
        static void setColors(Color colors[], int n);

        // Renders the screen rectangle
        static void render();
};
