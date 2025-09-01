#ifndef __CAMERA
#define __CAMERA

#include "Helpers.h"

namespace Eng {
    class Camera {

    public:
        Camera();
        Camera(const Camera& copy) = delete;
        Camera& operator=(const Camera& copy) = delete;
        Camera(Camera&& move) = delete;
        Camera& operator=(Camera&& move) = delete;
        ~Camera();

        mat4 view;
        mat4 projection;
        void setOrtho(const float& left, const float& right, const float& bottom, const float& top, const float& near, const float& far);
        void setProj(const float& fovY, const float& aspect, const float& near, const float& far);

        void setViewDirection(const vec3& position, const vec3& direction, const vec3& up={0.0f, -1.0f, 0.0f});
        void setViewTarget(const vec3& position, const vec3& target, const vec3& up={0.0f, -1.0f, 0.0f});
        void setViewYXZ(const vec3& position, const vec3& rotation);
    };
}

#endif// __CAMERA