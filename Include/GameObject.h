#ifndef ENG_GAMEOBJECT
#define ENG_GAMEOBJECT

#include "Helpers.h"
#include "Mesh.h"

namespace Eng {
    using id_t = unsigned int;
    struct TransformComponent {
        vec3 position{0.0f, 0.0f, 0.0f};
        vec3 scale{1.0f, 1.0f, 1.0f};
        vec3 rotation{0.0f, 0.0f, 0.0f};
        mat4 getTransformMat() const;
        mat3 getNormalMat() const;
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

        Mesh* mesh;
        TransformComponent transform;
    };
}

#endif