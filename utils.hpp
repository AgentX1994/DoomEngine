//
//  utils.hpp
//  OpenGL Renderer
//
//  Created by John Asper on 2/12/16.
//
//

#ifndef utils_hpp
#define utils_hpp

#include "main.hpp"

float degrees_to_radians(float a);
float radians_to_degrees(float a);

char *readSource(const char *file);

void mat4PrettyPrint(glm::mat4 m);

void vec3PrettyPrint(glm::vec3 v);

#define glCheckErrors(x) glCheckErrorsImpl(x, __FILE__, __LINE__)
void glCheckErrorsImpl(std::string description, std::string filename, int line);

void APIENTRY openglCallbackFunction(GLenum source,
                                     GLenum type,
                                     GLuint id,
                                     GLenum severity,
                                     GLsizei length,
                                     const GLchar* message,
                                     const void* userParam);
#endif /* utils_hpp */
