#include "main.hpp"
#include "program.hpp"
#include "screen.hpp"

void error_callback(int error, const char* des)
{
    std::cerr << "ERROR: " << des << std::endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    switch(key)
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, true);
            }
            break;
        default:
            switch (action)
            {
                case GLFW_PRESS:
                    std::cout << "Key pressed: " << scancode << std::endl;
                    break;
                case GLFW_RELEASE:
                    std::cout << "Key released: " << scancode << std::endl;
                    break;
            }
    }
}
int main()
{
    // Initialize glfw
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize glfw!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);

    // Set window settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

    if (!window)
    {
        std::cerr << "Could not create glfw window!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSwapInterval(1);

    uint32_t width, height;
    glfwGetFramebufferSize(window, (int*)&width, (int*)&height);
    glViewport(0, 0, width, height);

    screen::init();

    glClearColor(0.5, 0.5, 0.5, 1.0);
    double lastFrameTime = glfwGetTime();

    uint32_t frames = 0;
    double fps_time = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        double curFrameTime = glfwGetTime();
        double delta = curFrameTime - lastFrameTime;
        lastFrameTime = curFrameTime;
        fps_time += delta;
        frames++;
        if (fps_time > 1.0)
        {
            std::cout << frames << " fps" << std::endl;
            frames = 0;
            fps_time -= 1.0;
        }

        glfwGetFramebufferSize(window, (int*)&width, (int*)&height);
        glViewport(0, 0, width, height);
        //float ratio = width / (float) height;

        //render here
        screen::render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
