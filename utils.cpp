//
//  utils.cpp
//  OpenGL Renderer
//
//  Created by John Asper on 2/12/16.
//
//

#include "utils.hpp"

float degrees_to_radians(float a)
{
    return a*M_PI/180;
}

float radians_to_degrees(float a)
{
    return a*180/M_PI;
}

char *readSource(const char *file){
    char *text = NULL;
    if(file != NULL){
        FILE *f = fopen(file, "r");
        
        if (f != NULL){
            fseek(f, 0, SEEK_END);
            uint64_t len = ftell(f);
            rewind(f);
            
            if (len > 0) {
                text = new char[len+1];
                len = fread(text, sizeof(char), len, f);
                text[len] = 0;
            }
            fclose(f);
        }
    }
    return text;
}

void mat4PrettyPrint(glm::mat4 m){
    std::cout << "[" << std::endl;
    for (uint8_t i = 0; i < 4; i++) {
        std::cout << "[";
        for (uint8_t j = 0; j < 4; j++) {
            std::cout << *(&m[i][j]);
            if (j < 3) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "]" << std::endl;
}

void vec3PrettyPrint(glm::vec3 v){
    std::cout << "(";
    for (uint8_t i = 0; i < 3; i++){
        std::cout << v[i];
        if (i < 2) {
            std::cout << ", ";
        }
    }
    std::cout << ")" << std::endl;
}

void glCheckErrors(std::string loc){
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR){
        std::cout << "OPEN GL ERROR at location " << loc << "!" << std::endl;
        switch (err) {
            case GL_INVALID_ENUM:
                std::cout << "\tInvalid Enum";
                break;
            case GL_INVALID_VALUE:
                std::cout << "\tInvalid Value";
                break;
            case GL_INVALID_OPERATION:
                std::cout << "\tInvalid Operation";
                break;
            case GL_OUT_OF_MEMORY:
                std::cout << "\tOut of Memory Error";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cout << "\tInvalid Framebuffer Operation";
                break;
            default:
                std::cout << "\tUnknown Error!";
                break;
        }
        std::cout << std::endl << std::endl;
    }
}

/*
void APIENTRY openglCallbackFunction(GLenum source,
                                     GLenum type,
                                     GLuint id,
                                     GLenum severity,
                                     GLsizei length,
                                     const GLchar* message,
                                     const void* userParam){
    
    std::cout << "---------------------opengl-callback-start------------" << std::endl;
    std::cout << "message: "<< message << std::endl;
    std::cout << "type: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "OTHER";
            break;
    }
    std::cout << std::endl;
    
    std::cout << "id: " << id << std::endl;
    std::cout << "severity: ";
    switch (severity){
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "LOW";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "HIGH";
            break;
    }
    std::cout << std::endl;
    std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}
*/
