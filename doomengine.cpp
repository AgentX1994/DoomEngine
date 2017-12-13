#include "main.hpp"
#include "program.hpp"
#include "screen.hpp"
#include "input.hpp"

// A simple error callback that just echos errors to the cerr stream
void error_callback(int error, const char* des)
{
    std::cerr << "ERROR: " << des << std::endl;
}

// Temporary direction vector for raycasting
glm::vec2 pos(0,0);
glm::vec2 dir = glm::normalize(glm::vec2(1,1));
float fov = degrees_to_radians(60);
float turn_amount = degrees_to_radians(5);

float move_speed = 0.05;
float turn_speed = degrees_to_radians(2);

// Walls for testing the ray tracing
typedef struct wall {
    glm::vec2 p1;
    glm::vec2 p2;
    glm::vec3 color;
} wall;

const float box_radius = 10; // size of box from 0,0 to a corner
glm::vec2 wall_points[] = {
    glm::vec2(-box_radius,-box_radius),
    glm::vec2(-box_radius, box_radius),
    glm::vec2( box_radius,-box_radius),
    glm::vec2( box_radius, box_radius)
};

glm::vec3 COLOR_RED     (1.0, 0.0, 0.0);
glm::vec3 COLOR_WHITE   (1.0, 1.0, 1.0);
glm::vec3 COLOR_BLUE    (0.0, 0.0, 1.0);
glm::vec3 COLOR_GREEN   (0.0, 1.0, 0.0);
glm::vec3 COLOR_PINK    (1.0, 0.5, 0.5);
glm::vec3 COLOR_BLACK   (0.0, 0.0, 0.0);

wall walls[] = {
    {wall_points[0], wall_points[1], COLOR_WHITE},
    {wall_points[1], wall_points[3], COLOR_RED},
    {wall_points[2], wall_points[3], COLOR_BLUE},
    {wall_points[0], wall_points[2], COLOR_GREEN},
};

int num_walls = sizeof(walls)/sizeof(wall);

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

void get_closest_wall_one_ray(glm::vec2 p, glm::vec2 v, float &dist, glm::vec3 &color, wall map[], int num_walls)
{
    dist = INFINITY;
    color = COLOR_BLACK;
    for (int j = 0; j < num_walls; j++)
    {
        float d = cast_ray(p, v, map[j].p1, map[j].p2);
        if (d < dist)
        {
            dist = d;
            color = map[j].color;
        }
    }
}

void calc_closest_walls(float buffer[], glm::vec3 color_buffer[], int n, wall map[], int num_walls)
{
    float fov_2 = fov/2;
    float turn_angle = -fov/n;
    glm::vec2 v = glm::rotate(dir, fov_2);
    for (int i = 0; i < n; i++)
    {
        get_closest_wall_one_ray(pos, v, buffer[i], color_buffer[i], map, num_walls);
        v = glm::rotate(v, turn_angle);
    }
}

int main()
{
    std::cout << "Num walls: " << num_walls << std::endl;
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
    glfwSwapInterval(1);

    uint32_t width, height;
    glfwGetFramebufferSize(window, (int*)&width, (int*)&height);
    glViewport(0, 0, width, height);

    screen::init(); // Prepare screen for rendering
    input userInput(window);

    glClearColor(0.5, 0.5, 0.5, 1.0);
    double lastFrameTime = glfwGetTime();

    uint32_t frames = 0;
    double fps_time = 0.0;

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

        userInput.pollInput();

        if (userInput.getExitPressed())
        {
            glfwSetWindowShouldClose(window, true);
        }
        if (userInput.getMoveForward())
        {
            float dist; glm::vec3 color;
            get_closest_wall_one_ray(pos, dir, dist, color, walls, num_walls);
            if (dist > move_speed)
                pos += dir*move_speed;
        }
        if (userInput.getMoveBackward())
        {
            float dist; glm::vec3 color;
            get_closest_wall_one_ray(pos, -dir, dist, color, walls, num_walls);
            if (dist > move_speed)
                pos -= dir*move_speed;
        }
        if (userInput.getMoveLeft())
        {
            glm::vec2 move_dir = glm::rotate(dir, (float)M_PI/2);
            float dist; glm::vec3 color;
            get_closest_wall_one_ray(pos, move_dir, dist, color, walls, num_walls);
            if (dist > move_speed)
                pos += move_dir*move_speed;
        }
        if (userInput.getMoveRight())
        {
            glm::vec2 move_dir = glm::rotate(dir, (float)-M_PI/2);
            float dist; glm::vec3 color;
            get_closest_wall_one_ray(pos, move_dir, dist, color, walls, num_walls);
            if (dist > move_speed)
                pos += move_dir*move_speed;
        }
        if (userInput.getTurnLeft())
        {
            dir = glm::rotate(dir, turn_speed);
        }
        if (userInput.getTurnRight())
        {
            dir = glm::rotate(dir, -turn_speed);
        }

        // fill distance and color buffers
        calc_closest_walls(distances, textures, width, walls, num_walls);

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
