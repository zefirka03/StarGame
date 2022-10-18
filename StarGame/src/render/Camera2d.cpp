#include "Camera2d.h"

namespace air {

    Camera2d::Camera2d(float _width, float _height) {
        this->resize(_width, _height);

        this->transform.position = glm::vec2(0, 0);
        this->transform.origin = glm::vec2(0, 0);
        this->transform.scale = glm::vec2(1);
    }

    glm::mat4 Camera2d::getProjection() {
        view = glm::translate(glm::mat4(1), glm::vec3(transform.origin, 0));
        view = glm::scale(view, glm::vec3(transform.scale, 0));
        view = glm::translate(view, -glm::vec3(transform.position, 0));
        return projection * view;
    }

    Camera2d::Transform2d& Camera2d::getTransform() {
        return this->transform;
    }

    void Camera2d::setPosition(glm::vec2 _val) {
        transform.position = _val;
    }

    void Camera2d::setOrigin(glm::vec2 _val) {
        transform.origin = _val;
    }

    void Camera2d::setScale(glm::vec2 _val) {
        transform.scale = _val;
    }

    void Camera2d::setSize(glm::vec2 _val) {
        resize(_val.x, _val.y);
        transform.size = _val;
    }


    void Camera2d::resize(float _width, float _height) {
        this->width = _width;
        this->height = _height;

        transform.size = glm::vec2(_width, _height);

        this->projection = glm::ortho(0.f, this->width, this->height, 0.f);
        this->view = glm::mat4(1);
    }

    void Camera2d::move(glm::vec2 factor) {
        transform.position += factor;
    }

    void Camera2d::scale(float factor) {
        transform.scale *= glm::vec2(factor);
    }



    Camera2d::~Camera2d() {

    }

}