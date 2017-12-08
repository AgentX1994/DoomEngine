#include "screen.hpp"

// Initialize/declare static variables
bool screen::initialized = false;
GLfloat screen::vertices[] = {
    //      x,     y,    z
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f
    };
GLuint screen::indices[] = {
        0, 1, 2,
        2, 1, 3
    };
uint32_t screen::vaoID;
uint32_t screen::vboID;
uint32_t screen::iboID;
std::shared_ptr<program> screen::shader;

void screen::init()
{
    glCheckErrors("Starting init");
    shader = std::make_shared<program>("default.vert", "default.frag");
    glCheckErrors("Created shader");
    glBindFragDataLocation(shader->getID(), 0, "outColor");
    glCheckErrors("bindFragData");

    // Create simple square
    glGenVertexArrays(1, (GLuint *)&vaoID);
    glBindVertexArray(vaoID);

    glCheckErrors("created vao");

    // create vertex buffer
    glGenBuffers(1, (GLuint *)&vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, 3*4*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    GLint posAttrib = glGetAttribLocation(shader->getID(), "vert_position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glCheckErrors("created vbo");

    // create index buffer
    glGenBuffers(1, &iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLuint), indices, GL_STATIC_DRAW);

    glCheckErrors("created ibo");
    initialized = true;
}

void screen::render()
{
    if (!initialized)
    {
        std::cerr << "screen::render -  ERROR: trying to render before initializing!" << std::endl;
        return;
    }
    shader->use();
    glCheckErrors("used shader");

    glBindVertexArray(vaoID);
    glCheckErrors("bind vao");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glCheckErrors("bind ibo");

    // bind uniforms
    glm::mat4 proj = glm::mat4(); //glm::perspective(60.0f, ratio, 0.1f, 100.f);
    glm::mat4 view = glm::mat4();
    glm::mat4 model = glm::mat4();

    GLint projLoc = glGetUniformLocation(shader->getID(), "mProj");
    GLint viewLoc = glGetUniformLocation(shader->getID(), "mView");
    GLint modelLoc = glGetUniformLocation(shader->getID(), "mWorld");
    glCheckErrors("get Uniforms");

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glCheckErrors("set Uniforms");

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glCheckErrors("draw tris");

    shader->unuse();
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckErrors("unuse shader");
}
