#include "GameObject.h"

namespace Eng {
    mat4 Transform::getMat() const {
        return glm::scale(glm::translate(mat4(1), position), scale)*glm::rotate(rotation, vec3(0.0f, 0.0f, 1.0f));
    }

    GameObject::GameObject(id_t _id) : id(_id) {

    }
    id_t currentId = 0;
    GameObject GameObject::createGameObject() {
        return GameObject(currentId++);
    }
    GameObject::~GameObject() {
        
    }
}