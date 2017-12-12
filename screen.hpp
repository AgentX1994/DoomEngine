#pragma once
#include "main.hpp"
#include "program.hpp"

class screen
{
    private:
        static GLfloat vertices[];
        static GLuint indices[]; 
        static GLfloat uvs[];
        static std::shared_ptr<program> shader; 

        static uint32_t vaoID;
        static uint32_t vboID;
        static uint32_t iboID;
        static uint32_t uvBuffer;

        static GLuint distTextureID;

        static bool initialized;
    public:
        static void init();
        static void setDistances(float d[], int n);
        static void render();
};
