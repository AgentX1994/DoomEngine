#pragma once
#include "main.hpp"

class program
{
    private:
        uint32_t id;
        uint32_t shaderID;
        uint32_t shaderVP;
        uint32_t shaderFP;

        void init(const char* vsFile, const char *fsFile);
        program();
        program(const program &s);
    public:
        program (const char* vertexFile, const char* fragmentFile);
        ~program();

        void use();
        void unuse();
        uint32_t getID();
};
