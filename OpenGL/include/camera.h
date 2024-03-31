#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "glad/glad.h"

class Camera {
public:
    explicit Camera(const glm::vec3& position=glm::vec3(0.0f, 0.0f, 0.0f),
           const glm::vec3& direction=glm::vec3(0.0f, 0.0f, -1.0f),
           const glm::vec3& up=glm::vec3(0.0f, 1.0f, 0.0f));

    void lookAt(const glm::vec3& target);

    void move(float x, float y, float z);

    void moveTo(const glm::vec3& target);

    void rotate(float pitch, float yaw, float roll);

    glm::vec3 position;
    glm::vec3 front, right, up;
    glm::mat4 viewMatrix;
};


#endif //OPENGL_CAMERA_H
