#include "camera.h"

Camera *currentCamera;

Camera::Camera(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &up) {
    this->position = position;
    this->front = glm::normalize(direction);
    this->right = glm::normalize(glm::cross(direction, up));
    this->up = glm::normalize(glm::cross(right, direction));
    this->view = glm::lookAt(position, position + direction, up);
    this->projection = glm::mat4(1.f);
}

void Camera::use() {
    currentCamera = this;
}

void Camera::setProjection(float aspectRatio, float radiusOfView) {
    this->projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, radiusOfView);
}

glm::mat4 Camera::getMatrix(bool useProjection) {
    if (useProjection)
        return this->projection * this->view;
    return this->view;
}

void Camera::move(float x, float y, float z) {
    moveTo(position - front * z + right * x + up * y);
}

void Camera::moveTo(const glm::vec3 &target) {
    this->position = target;
    this->view = glm::lookAt(position, position + front, up);
}

void Camera::lookAt(const glm::vec3 &target) {
    this->front = glm::normalize(target - position);
    this->right = glm::normalize(glm::cross(front, up));
    this->up = glm::normalize(glm::cross(right, front));
    this->view = glm::lookAt(position, target, up);
}

void Camera::rotate(float pitch, float yaw, float roll) {
    glm::mat4 transform4(1.0f);

    transform4 = glm::rotate(transform4, glm::radians(-roll), front);
    transform4 = glm::rotate(transform4, glm::radians(pitch), right);
    transform4 = glm::rotate(transform4, glm::radians(yaw), up);

    glm::mat3 transform3(transform4);

    this->front = glm::normalize(transform3 * front);
    this->right = glm::normalize(transform3 * right);
    this->up = glm::normalize(transform3 * up);
    this->view = glm::lookAt(position, position + front, up);
}