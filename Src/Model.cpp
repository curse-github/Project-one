#include "Model.h"

namespace Eng {
    
    Model::Model(Device* _device, const std::vector<Vertex>& vertices) : device(_device) {
        vertexCount = static_cast<unsigned int>(vertices.size());
        assert((vertices.size() >= 3) && "Vertex count must be at least 3");
        // create GPU buffer linked with CPU buffer
        VkDeviceSize bufferSize = sizeof(Vertex) * vertexCount;
        device->createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);
        void* data;
        vkMapMemory(device->device, vertexBufferMemory, 0, bufferSize, 0, &data);
        // mode data in
        memcpy(data, vertices.data(), static_cast<unsigned int>(bufferSize));
        // unlink GPU and CPU
        vkUnmapMemory(device->device, vertexBufferMemory);
    }
    Model::~Model() {
        vkDestroyBuffer(device->device, vertexBuffer, nullptr);
        vkFreeMemory(device->device, vertexBufferMemory, nullptr);
    }
    void Model::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }
    void Model::draw(VkCommandBuffer commandBuffer) {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }
    std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }
    std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
        // "VK_FORMAT_R32_SFLOAT" is 1 floats
        // "VK_FORMAT_R32G32_SFLOAT" is 2 floats
        // "VK_FORMAT_R32G32B32_SFLOAT" is 3 floats
        // position attribute
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);
        // color attribute
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        // normal attribute
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, normal);
        return attributeDescriptions;
    }
};