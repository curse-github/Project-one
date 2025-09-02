#ifndef __MESH
#define __MESH

#include "Helpers.h"
#include "Device.h"

namespace Eng {
    class Mesh {
        Device* device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        unsigned int vertexCount;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        unsigned int indexCount;
        bool hasIndexBuffer = false;
    public:
        struct Vertex {
            vec3 position{};
            vec2 uv{};
            vec3 normal{};
            vec3 color{};

            bool operator==(const Vertex &other) const {
                return (position == other.position) && (uv == other.uv) && (normal == other.normal) && (color == other.color);
            }
            
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
        struct MeshData {
            std::vector<Vertex> vertices{};
            std::vector<unsigned int> indices{};

            MeshData() {};
            MeshData(std::vector<Vertex> _vertices) : vertices(_vertices) {};
            MeshData(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices) : vertices(_vertices), indices(_indices) {};
            MeshData(const MeshData& copy) = default;
            MeshData& operator=(const MeshData& copy) = default;
            MeshData(MeshData&& move) = default;
            MeshData& operator=(MeshData&& move) = default;
            ~MeshData() = default;
        };
        Mesh(Device* _device, const MeshData& meshData);
        Mesh(const Mesh& copy) = delete;
        Mesh& operator=(const Mesh& copy) = delete;
        Mesh(Mesh&& move) = default;
        Mesh& operator=(Mesh&& move) = default;
        ~Mesh();

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
    };
};

#endif