#pragma once

#include "../glIncl.h"

namespace air {

    class Camera2d {
        struct Transform2d;
    public:
        //Camera2d();
        Camera2d(float _width, float _height);
        glm::mat4 getProjection();
        void move(glm::vec2 factor);
        void scale(float factor);
        void resize(float _width, float _height);
        Transform2d& getTransform();
        void setPosition(glm::vec2 _val);
        void setOrigin(glm::vec2 _val);
        void setSize(glm::vec2 _val);
        void setScale(glm::vec2 _val);

        ~Camera2d();
    private:
        float width, height;
        struct Transform2d {
            glm::vec2 position;
            glm::vec2 origin;
            glm::vec2 scale;
            glm::vec2 size;
        }transform;

        glm::mat4 projection;
        glm::mat4 view;
    };
}