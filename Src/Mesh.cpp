#include "Mesh.h"

namespace Eng {
    Mesh::Mesh(Device* _device, const Mesh::MeshData& meshData) : device(_device) {
        // create vertex buffer
        vertexCount = static_cast<unsigned int>(meshData.vertices.size());
        assert((meshData.vertices.size() >= 3) && "Vertex count must be at least 3");
        // create GPU coherent memory buffer linked with a CPU buffer
        VkDeviceSize vertexBufferSize = sizeof(meshData.vertices[0]) * vertexCount;
        VkBuffer tempBuffer;
        VkDeviceMemory tempBufferMemory;
        device->createBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, tempBuffer, tempBufferMemory);
        void* vertexData;
        vkMapMemory(device->device, tempBufferMemory, 0, vertexBufferSize, 0, &vertexData);
        // move vertex data in
        memcpy(vertexData, meshData.vertices.data(), static_cast<unsigned int>(vertexBufferSize));
        // unlink GPU and CPU
        vkUnmapMemory(device->device, tempBufferMemory);
        // move vertex data into GPU local memory, from the coherent memory
        device->createBuffer(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
        device->copyBuffer(tempBuffer, vertexBuffer, vertexBufferSize);
        // cleanup GPU coherent memory buffer
        vkDestroyBuffer(device->device, tempBuffer, nullptr);
        vkFreeMemory(device->device, tempBufferMemory, nullptr);

        // create index buffer
        indexCount = static_cast<unsigned int>(meshData.indices.size());
        hasIndexBuffer = indexCount > 0;
        if (!hasIndexBuffer) return;
        assert((meshData.indices.size() >= 3) && "Index count must be either 0, or least 3");
        // create GPU coherent memory buffer linked with a CPU buffer
        VkDeviceSize indexBufferSize = sizeof(meshData.indices[0]) * indexCount;
        device->createBuffer(indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, tempBuffer, tempBufferMemory);
        void* indexData;
        vkMapMemory(device->device, tempBufferMemory, 0, indexBufferSize, 0, &indexData);
        // move index data in
        memcpy(indexData, meshData.indices.data(), static_cast<unsigned int>(indexBufferSize));
        // unlink GPU and CPU
        vkUnmapMemory(device->device, tempBufferMemory);
        // move index data into GPU local memory, from the coherent memory
        device->createBuffer(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
        device->copyBuffer(tempBuffer, indexBuffer, indexBufferSize);
        // cleanup GPU coherent memory buffer
        vkDestroyBuffer(device->device, tempBuffer, nullptr);
        vkFreeMemory(device->device, tempBufferMemory, nullptr);
    }
    // https://en.wikipedia.org/wiki/Wavefront_.obj_file
    Mesh::~Mesh() {
        vkDestroyBuffer(device->device, vertexBuffer, nullptr);
        vkFreeMemory(device->device, vertexBufferMemory, nullptr);
        if (hasIndexBuffer) {
            vkDestroyBuffer(device->device, indexBuffer, nullptr);
            vkFreeMemory(device->device, indexBufferMemory, nullptr);
        }
    }
    
    std::vector<VkVertexInputBindingDescription> Mesh::Vertex::getBindingDescriptions() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }
    std::vector<VkVertexInputAttributeDescription> Mesh::Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        // "VK_FORMAT_R32_SFLOAT" is 1 floats
        // "VK_FORMAT_R32G32_SFLOAT" is 2 floats
        // "VK_FORMAT_R32G32B32_SFLOAT" is 3 floats
        attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
        attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32_SFLOAT   , offsetof(Vertex, uv)});
        attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
        attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
        return attributeDescriptions;
    }

    void Mesh::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        if (hasIndexBuffer)
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
    void Mesh::draw(VkCommandBuffer commandBuffer) {
        if (hasIndexBuffer)
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        else
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }
};