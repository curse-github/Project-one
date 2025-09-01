#ifndef __GAMEOBJECT
#define __GAMEOBJECT

#include "Helpers.h"
#include "Model.h"

namespace Eng {
    using id_t = unsigned int;
    struct TransformComponent {
        vec3 position;
        vec3 scale;
        vec3 rotation;
        mat4 getMat() const;
    };
    class GameObject {
        id_t id;
        GameObject(id_t _id);
    public:
        static GameObject createGameObject();
        GameObject(const GameObject& copy) = delete;
        GameObject& operator=(const GameObject& copy) = delete;
        GameObject(GameObject&& move) = default;
        GameObject& operator=(GameObject&& move) = default;
        ~GameObject();

        Model* model;
        TransformComponent transform;
    };
}

#endif