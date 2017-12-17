#pragma once
#include "main.hpp"

class camera
{
    private:
        glm::vec2 position{0,0};
        glm::vec2 viewDirection{1,0};

    public:
        camera() {}
        camera(glm::vec2 p, glm::vec2 d) : position(p), viewDirection(d) {}

        glm::vec2 getPosition() { return position; }
        glm::vec2 getViewDirection() { return viewDirection; }

        float getPositionX() { return position.x; }
        float getPositionY() { return position.y; }

        void setPosition(glm::vec2 p) { position = p; }
        void setViewDirection(glm::vec2 d) { viewDirection = d; }

        void move(float dist)
        {
            position += viewDirection*dist;
        }

        void move(glm::vec2 dir, float dist)
        {
            position += dir*dist;
        }

        void move(glm::vec2 dir)
        {
            move(dir, 1);
        }

        void move(float angle, float dist)
        {
            position += glm::rotate(viewDirection, angle)*dist;
        }

        void turn(float angle)
        {
            viewDirection = glm::rotate(viewDirection, angle);
        }
};
