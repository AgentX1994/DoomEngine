#pragma once
// Standard Includes
#include <iostream>
#include <algorithm>
#include <cinttypes>
#include <cmath>
#include <memory>

// OpenGl includes
#ifdef __apple__

#include <OpenGL/GL3.h>

#elif __linux__

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#else

#error "UNSUPPORTED TARGET"

#endif

#include <GLFW/glfw3.h>

// GLM includes (math types and functions)
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Utility Functions
#include "utils.hpp"
