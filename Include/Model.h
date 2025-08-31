#ifndef __MODEL
#define __MODEL

#include "Helpers.h"
#include "Device.h"

namespace Eng {
    class Model {
        Device* device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        unsigned int vertexCount;
    public:
        struct Vertex {
            vec2 position;
            vec3 color;
            
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
        Model(Device* _device, const std::vector<Vertex>& vertices);
        Model(const Model& copy) = delete;
        Model& operator=(const Model& copy) = delete;
        Model(Model&& move) = delete;
        Model& operator=(Model&& move) = delete;
        ~Model();

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
    };
};

#endif