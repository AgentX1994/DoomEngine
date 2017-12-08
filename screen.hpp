#pragma once
#include "main.hpp"
#include "program.hpp"

class screen
{
    private:
        static GLfloat vertices[];
        static GLuint indices[]; 
        static program shader; 

        static uint32_t vaoID;
        static uint32_t vboID;
        static uint32_t iboID;

        static bool initialized;
    public:
        static void init();
        static void render();
};
