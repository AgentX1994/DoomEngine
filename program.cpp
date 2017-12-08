#include "program.hpp"
#include <vector>

program::program()
{
}

program::program (const char* vertexFile, const char* fragmentFile)
{
    init(vertexFile, fragmentFile);
}

program::~program()
{
    glDeleteShader(shaderFP);
    glDeleteShader(shaderVP);
    glDeleteProgram(shaderID);
}

void program::use()
{
    if (glIsProgram(shaderID) != GL_TRUE)
    {
        std::cerr << "GL ERROR: THERE IS NO PROGRAM " << shaderID << std::endl;
    }
    glUseProgram(shaderID);
}

void program::unuse()
{
    glUseProgram(0);
}

uint32_t program::getID()
{
    return shaderID;
}

void program::init(const char* vsFile, const char* fsFile)
{
    // Create the shaders
    shaderVP = glCreateShader(GL_VERTEX_SHADER);
    shaderFP = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vp = readSource(vsFile);
    const char* fp = readSource(fsFile);

    if (vp == nullptr || fp == nullptr)
    {
        std::cerr << "program::init ERROR: ONE OR BOTH FILES NOT FOUND!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glShaderSource(shaderVP, 1, &vp, 0);
    glShaderSource(shaderFP, 1, &fp, 0);

    delete [] vp;
    delete [] fp;

    // Compile
    glCompileShader(shaderVP);
    // error checking
    GLint isCompiled = 0;
    glGetShaderiv(shaderVP, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderVP, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shaderVP, maxLength, &maxLength, &errorLog[0]);
        std::cerr << "Vertex Shader Compilation Error:" << std::endl;
        for (auto it = errorLog.begin(); it != errorLog.end(); it++)
        {
            std::cerr << *it;
        }

        std::cerr << std::endl;
        glDeleteShader(shaderVP); // no leaks
        exit(EXIT_FAILURE);
    }

    // Compile
    glCompileShader(shaderFP);
    // error checking
    isCompiled = 0;
    glGetShaderiv(shaderFP, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderFP, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shaderFP, maxLength, &maxLength, &errorLog[0]);
        std::cerr << "Fragment Shader Compilation Error:" << std::endl;
        for (auto it = errorLog.begin(); it != errorLog.end(); it++)
        {
            std::cerr << *it;
        }

        std::cerr << std::endl;
        glDeleteShader(shaderVP); // no leaks
        glDeleteShader(shaderFP);
        exit(EXIT_FAILURE);
    }

    // Create shader program
    shaderID = glCreateProgram();

    // Attach shaders
    glAttachShader(shaderID, shaderVP);
    glAttachShader(shaderID, shaderFP);

    glLinkProgram(shaderID);

    GLint isLinked;
    glGetProgramiv(shaderID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength;
        glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

        // The program is useless
        glDeleteProgram(shaderID);

        for (auto it = infoLog.begin(); it != infoLog.end(); it++)
        {
            std::cerr << *it;
        }
        std::cerr << std::endl;
        glDeleteShader(shaderVP);
        glDeleteShader(shaderFP);

        exit(EXIT_FAILURE);
    }

    // shaders can be safely detached
    glDetachShader(shaderID, shaderVP);
    glDetachShader(shaderID, shaderFP);

    glDeleteShader(shaderVP);
    glDeleteShader(shaderFP);

    std::cout << "Created shader program #" << shaderID << ": " << vsFile << " and " << fsFile << std::endl;
    glCheckErrors("end of shader init");
}
