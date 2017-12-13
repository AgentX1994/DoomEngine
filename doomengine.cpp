#include "main.hpp"
#include "program.hpp"
#include "screen.hpp"

// A simple error callback that just echos errors to the cerr stream
void error_callback(int error, const char* des)
{
    std::cerr << "ERROR: " << des << std::endl;
}

// Temporary direction vector for raycasting
glm::vec2 dir = glm::normalize(glm::vec2(1,1));
float fov = degrees_to_radians(60);
float turn_amount = degrees_to_radians(5);

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
        case GLFW_KEY_A:
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                dir = glm::rotate(dir, turn_amount);
            }
            break;
        case GLFW_KEY_D:
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                dir = glm::rotate(dir, -turn_amount);
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

// Walls for testing the ray tracing
typedef struct wall {
    glm::vec2 p1;
    glm::vec2 p2;
    glm::vec3 color;
} wall;

const float box_radius = 1; // size of box from 0,0 to a corner
glm::vec2 wall_points[] = {
    glm::vec2(-box_radius,-box_radius),
    glm::vec2(-box_radius, box_radius),
    glm::vec2( box_radius,-box_radius),
    glm::vec2( box_radius, box_radius)
};

glm::vec3 COLOR_RED(1,0,0);
glm::vec3 COLOR_WHITE(1,1,1);
glm::vec3 COLOR_BLUE(0, 0, 1);
glm::vec3 COLOR_GREEN(0, 1, 0);
glm::vec3 COLOR_PINK(1, 0.5, 0.5);

wall walls[] = {
    {wall_points[0], wall_points[1], COLOR_WHITE},
    {wall_points[1], wall_points[3], COLOR_RED},
    {wall_points[2], wall_points[3], COLOR_BLUE},
    {wall_points[0], wall_points[2], COLOR_GREEN},
};

float cross_2d(glm::vec2 a, glm::vec2 b)
{
    return a.x*b.y - a.y*b.x;
}

float eps = 1e-4;
// Finds the distance from point p to the line q1 q2 in the direction r
// Assumes r is normalized
float cast_ray(glm::vec2 p, glm::vec2 r, glm::vec2 q1, glm::vec2 q2)
{
    glm::vec2 s = q2-q1;
    float r_cross_s = cross_2d(r,s);
    if (std::abs(r_cross_s) > eps)
    {
        glm::vec2 q_minus_p = q1-p;
        float qp_cross_r = cross_2d(q_minus_p, r);
        float qp_cross_s = cross_2d(q_minus_p, s);
        float t = qp_cross_s/r_cross_s;
        float u = qp_cross_r/r_cross_s;
        if (t > 0 && u > 0 && u < 1)
        {
            return t;
        } else
        {
            return INFINITY;
        }
    } else
    {
        return INFINITY;
    }
}

void calc_closest_wall(float buffer[], glm::vec3 color_buffer[], int n, wall map[], int num_walls)
{
    float fov_2 = fov/2;
    float turn_angle = -fov/n;
    glm::vec2 v = glm::rotate(dir, fov_2);
    //std::cout << "start vector: <" << v.x << "," << v.y << "}" << std::endl;
    for (int i = 0; i < n; i++)
    {
        //std::cout << "testing for intersections for vector: <" << v.x << "," << v.y << ">" << std::endl;
        float min_dist = INFINITY;
        glm::vec3 min_color = COLOR_PINK;
        for (int j = 0; j < num_walls; j++)
        {
            float dist = cast_ray(glm::vec2(0,0), v, map[j].p1, map[j].p2);
            if (dist < min_dist)
            {
                min_dist = dist;
                min_color = map[j].color;
            }
        }
        buffer[i] = min_dist;
        color_buffer[i] = min_color;
        v = glm::rotate(v, turn_angle);
    }
    //std::cout << "end vector: <" << v.x << "," << v.y << "}" << std::endl;
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

    //double phase = 0.0; // Phase for sin distance buffer testing
    float *distances = new float[width]; // per vertical line distance buffer
    glm::vec3 *textures = new glm::vec3[width]; // per vertical line color
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
        //gen_sin(distances, width, 0.01, phase);
        calc_closest_wall(distances, textures, width, walls, 4);

        /* std::cout << "["; */
        /* for (int i = 0; i < width; i++) */
        /* { */
        /*     std::cout << distances[i] << ","; */
        /* } */
        /* std::cout << "]" << std::endl; */

        //render here
        screen::setDistances(distances, width);
        screen::setColors(textures, width);
        screen::render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete [] distances;

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
