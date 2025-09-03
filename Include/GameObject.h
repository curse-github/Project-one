#ifndef ENG_GAMEOBJECT
#define ENG_GAMEOBJECT

#include "Helpers.h"
#include "Mesh.h"

namespace Eng {
    struct TransformComponent {
        vec3 position{0.0f, 0.0f, 0.0f};
        vec3 scale{1.0f, 1.0f, 1.0f};
        vec3 rotation{0.0f, 0.0f, 0.0f};
        mat4 getTransformMat() const;
        mat3 getNormalMat() const;
    };
    class GameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>;
        static GameObject createGameObject();
        GameObject(const GameObject& copy) = delete;
        GameObject& operator=(const GameObject& copy) = delete;
        GameObject(GameObject&& move) = default;
        GameObject& operator=(GameObject&& move) = default;
        ~GameObject();

        id_t id;
        Mesh* mesh;
        TransformComponent transform;
    private:
        GameObject(id_t _id);
    };
}

#endif