#include "Camera.h"

namespace Eng {
    Camera::Camera() {

    }
    Camera::~Camera() {
        
    }
    void Camera::setOrtho(const float& left, const float& right, const float& bottom, const float& top, const float& near, const float& far) {
        projection = mat4(1.0f);
        projection[0][0] = 2.f / (right - left);
        projection[1][1] = 2.f / (bottom - top);
        projection[2][2] = 1.f / (far - near);
        projection[3][0] = -(right + left) / (right - left);
        projection[3][1] = -(bottom + top) / (bottom - top);
        projection[3][2] = -near / (far - near);
    }
    void Camera::setProj(const float& fovY, const float& aspect, const float& near, const float& far) {
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFovY = glm::tan(fovY / 2.f);
        projection = mat4(0.0f);
        projection[0][0] = 1.f / (aspect * tanHalfFovY);
        projection[1][1] = 1.f / (tanHalfFovY);
        projection[2][2] = far / (far - near);
        projection[2][3] = 1.f;
        projection[3][2] = -(far * near) / (far - near);
    }
}