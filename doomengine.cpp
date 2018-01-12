#include "main.hpp"
#include "program.hpp"
#include "screen.hpp"
#include "input.hpp"
#include "camera.hpp"
#include "glyphcache.hpp"
#include <sstream>

// A simple error callback that just echos errors to the cerr stream
void error_callback(int error, const char* des)
{
    std::cerr << "ERROR: " << des << std::endl;
}

float fov = degrees_to_radians(60);
float turn_amount = degrees_to_radians(5);

float move_speed = 0.05;
float turn_speed = degrees_to_radians(2);

// Simple Colors to use for walls
Color COLOR_RED     (1.0, 0.0, 0.0, 1.0);
Color COLOR_WHITE   (1.0, 1.0, 1.0, 1.0);
Color COLOR_BLUE    (0.0, 0.0, 1.0, 1.0);
Color COLOR_GREEN   (0.0, 1.0, 0.0, 1.0);
Color COLOR_PINK    (1.0, 0.5, 0.5, 1.0);
Color COLOR_BLACK   (0.0, 0.0, 0.0, 1.0);

// Walls for testing the ray tracing
typedef struct wall {
    glm::vec2 p1;
    glm::vec2 p2;
    Color color;
} wall;

wall walls[] = {
    //    x1,   y1,       x2,   y2,        color
    { { -5.0,  5.0 }, {  5.0,  5.0 }, COLOR_BLUE  },
    { {  5.0,  5.0 }, {  5.0,  1.0 }, COLOR_RED   },
    { {  5.0,  1.0 }, { 10.0,  5.0 }, COLOR_GREEN },
    { { 10.0,  5.0 }, { 10.0, -5.0 }, COLOR_BLUE  },
    { { 10.0, -5.0 }, {  5.0, -1.0 }, COLOR_RED   },
    { {  5.0, -1.0 }, {  5.0, -5.0 }, COLOR_GREEN },
    { {  5.0, -5.0 }, { -5.0, -5.0 }, COLOR_BLUE  },
    { { -5.0, -5.0 }, { -5.0,  5.0 }, COLOR_RED   },
};

uint32_t num_walls = sizeof(walls)/sizeof(wall);

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

void get_closest_wall_one_ray(glm::vec2 p, glm::vec2 v, float &dist, Color &color, wall map[], uint32_t num_walls)
{
    dist = INFINITY;
    color = COLOR_BLACK;
    for (uint32_t j = 0; j < num_walls; j++)
    {
        float d = cast_ray(p, v, map[j].p1, map[j].p2);
        if (d < dist)
        {
            dist = d;
            color = map[j].color;
        }
    }
}

void calc_closest_walls(camera cam, float buffer[], Color color_buffer[], uint32_t n, wall map[], uint32_t num_walls)
{
    float fov_2 = fov/2;
    float turn_angle = -fov/n;
    glm::vec2 v = glm::rotate(cam.getViewDirection(), fov_2);
    for (uint32_t i = 0; i < n; i++)
    {
        get_closest_wall_one_ray(cam.getPosition(), v, buffer[i], color_buffer[i], map, num_walls);
        v = glm::rotate(v, turn_angle);
    }
}

FT_Face face;
// Function to draw a line of text
void render_text(glyphcache &cache, const char *text, float x, float y, float sx, float sy) {
    const char *p;

    for(p = text; *p; p++) {

        cache_entry e = cache.get(*p);

        FT_BitmapGlyph g = e.glyph;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,e.tex_id);
        float x2 = x + g->left * sx;
        float y2 = -y - g->top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;

        GLfloat box[4][4] = {
            {x2,     -y2    , 0, 0},
            {x2 + w, -y2    , 1, 0},
            {x2,     -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        x += (e.advancex/64) * sx;
        y += (e.advancey/64) * sy;
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

    GLFWwindow *window = glfwCreateWindow(1600, 1200, "Doom Engine", NULL, NULL);

    if (!window)
    {
        std::cerr << "Could not create glfw window!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup freetype
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cerr << "Could not init freetype library!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // load font for testing
    // TODO: More portable way of loading fonts
    const char* font = "/Users/john/Library/Fonts/Droid Sans Mono for Powerline.otf";
    if (FT_New_Face(ft, font, 0, &face))
    {
        std::cerr << "Could not load font!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glyphcache cache(face);

    FT_Set_Pixel_Sizes(face, 0, 48);

    uint32_t width, height;
    glfwGetFramebufferSize(window, (int*)&width, (int*)&height);
    glViewport(0, 0, width, height);

    screen::init(); // Prepare screen for rendering
    input userInput(window);

    program textShader("text.vert", "text.frag");

    // turn on alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCheckErrors("turn on alpha blending");

    // character texture
    glActiveTexture(GL_TEXTURE0);
    GLuint glyphTex;
    glGenTextures(1, &glyphTex);
    glBindTexture(GL_TEXTURE_2D, glyphTex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glCheckErrors("create glyph tex");

    GLuint glyphVAO;
    glGenVertexArrays(1, &glyphVAO);
    glBindVertexArray(glyphVAO);
    glCheckErrors("Create and bind glyph VAO");

    GLuint glyphVBO;
    glGenBuffers(1, &glyphVBO);
    glCheckErrors("Create glyph VBO");

    glBindBuffer(GL_ARRAY_BUFFER, glyphVBO);
    glCheckErrors("setup glyph vert attrib 1");
    GLuint posLoc = glGetAttribLocation(textShader.getID(), "position");
    glCheckErrors("setup glyph vert attrib 2");
    glVertexAttribPointer(posLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glCheckErrors("setup glyph vert attrib 3");
    glEnableVertexAttribArray(posLoc);
    glCheckErrors("setup glyph vert attrib 4");

    glBindVertexArray(0);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    double lastFrameTime = glfwGetTime();

    uint32_t frames = 0;
    double fps_time = 0.0;

    float *distances = new float[width]; // per vertical line distance buffer
    Color *textures = new Color[width]; // per vertical line color

    camera player;

    int fps = 0;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Simple fps counter
        double curFrameTime = glfwGetTime();
        double delta = curFrameTime - lastFrameTime;
        lastFrameTime = curFrameTime;
        fps_time += delta;
        frames++;
        fps = (int)(1/delta);
        if (fps_time > 1.0)
        {
            std::cout << frames << " fps" << std::endl;
            frames = 0;
            fps_time -= 1.0;
        }

        // Check if window has been resized
        uint32_t old_width = width;
        glfwGetFramebufferSize(window, (int*)&width, (int*)&height);
        glViewport(0, 0, width, height);

        if (old_width != width)
        {
            std::cout << "width changed" << std::endl;
            delete [] distances;
            distances = new float[width]; // Create new distance buffer
            delete [] textures;
            textures = new Color[width]; // and texture buffer
        }

        userInput.pollInput();

        if (userInput.getExitPressed())
        {
            glfwSetWindowShouldClose(window, true);
            continue;
        }
        if (userInput.getResetPressed())
        {
            player = camera();
        }
        if (userInput.getMoveForward())
        {
            float dist; Color color;
            get_closest_wall_one_ray(player.getPosition(), player.getViewDirection(), dist, color, walls, num_walls);
            if (dist > move_speed)
                player.move(move_speed);
        }
        if (userInput.getMoveBackward())
        {
            float dist; Color color;
            get_closest_wall_one_ray(player.getPosition(), -player.getViewDirection(), dist, color, walls, num_walls);
            if (dist > move_speed)
                player.move(-move_speed);
        }
        if (userInput.getMoveLeft())
        {
            glm::vec2 move_view_dir = glm::rotate(player.getViewDirection(), (float)M_PI/2);
            float dist; Color color;
            get_closest_wall_one_ray(player.getPosition(), move_view_dir, dist, color, walls, num_walls);
            if (dist > move_speed)
                player.move(M_PI/2, move_speed);
        }
        if (userInput.getMoveRight())
        {
            glm::vec2 move_dir = glm::rotate(player.getViewDirection(), (float)-M_PI/2);
            float dist; Color color;
            get_closest_wall_one_ray(player.getPosition(), move_dir, dist, color, walls, num_walls);
            if (dist > move_speed)
                player.move(-M_PI/2, move_speed);
        }
        if (userInput.getTurnLeft())
        {
            player.turn(turn_speed);
        }
        if (userInput.getTurnRight())
        {
            player.turn(-turn_speed);
        }

        // fill distance and color buffers
        calc_closest_walls(player, distances, textures, width, walls, num_walls);

        //render here
        screen::setDistances(distances, width);
        screen::setColors(textures, width);
        screen::render();

        // Render text
        glBindVertexArray(glyphVAO);
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, glyphTex);
        glCheckErrors("Bind glyph tex");
        textShader.use();

        GLuint colorLoc = glGetUniformLocation(textShader.getID(), "color");
        GLuint texLoc = glGetUniformLocation(textShader.getID(), "glyph");
        glUniform1i(texLoc, 0);
        glUniform4fv(colorLoc, 1, &COLOR_BLACK[0]);
        glCheckErrors("Setup glyph uniforms");

        float sx = 2.0/width;
        float sy = 2.0/height;
        std::stringstream s;
        s << fps << " fps " << delta << " ms per frame";
        render_text(cache, s.str().c_str(),
                -1 + 8 * sx,   1 - 50 * sy,    sx, sy);
        render_text(cache, "This is a nice long string that should really stress test the cache",
                -1 + 8 *sx, 1 - 100 * sy, sx, sy);
        render_text(cache, "The Quick Brown Fox Jumps Over The Lazy Dog",
                -1 + 8 * sx, 1 - 150 * sy, sx, sy);

        textShader.unuse();
        glBindVertexArray(0);
        glCheckErrors("Unbind glyph");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete [] distances;

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
