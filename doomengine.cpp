#include "main.hpp"
#include "program.hpp"
#include "screen.hpp"

// A simple error callback that just echos errors to the cerr stream
void error_callback(int error, const char* des)
{
    std::cerr << "ERROR: " << des << std::endl;
}

// A simple key callback that closes the window if esc is pressed, and otherwise
// simply echos the keycode to cout
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

// generates a sin wave in buffer, with the frequency freq, and starting phase
// phase.
// The sin wave will vary between 0 and 1 instead of -1 and 1
// Frequency is based on buffer length, so a frequency of 5.0 will generate a
// sin wave with 5 periods in the buffer
void gen_sin(float buffer[], int n, double freq, double &phase)
{
    double inc = 2.0*M_PI*freq/n;
    for (int i = 0; i < n; i++)
    {
        buffer[i] = std::sin(phase)/2 + 0.5;
        phase += inc;
        if (phase >= 2.0*M_PI) phase -= 2.0*M_PI;
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);                  // OpenGL 3.2
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Forward compatible
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Core

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);    // Let window be resized

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

    screen::init(); // Prepare screen for rendering

    glClearColor(0.5, 0.5, 0.5, 1.0);
    double lastFrameTime = glfwGetTime();

    uint32_t frames = 0;
    double fps_time = 0.0;

    double phase = 0.0; // Phase for sin distance buffer testing
    float *distances = new float[width]; // per pixel distance buffer

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Simple fps counter
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

        // Check if window has been resized
        int old_width = width;
        glfwGetFramebufferSize(window, (int*)&width, (int*)&height);
        glViewport(0, 0, width, height);

        if (old_width != width)
        {
            std::cout << "width changed" << std::endl;
            delete [] distances;
            distances = new float[width]; // Create new distance buffer
        }

        // fill distance buffer
        gen_sin(distances, width, 5.05, phase);

        //render here
        screen::setDistances(distances, width);
        screen::render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete [] distances;

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
