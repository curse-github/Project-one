#ifndef ENG_LOADER
#define ENG_LOADER

#include "Helpers.h"
#include "Mesh.h"
#include "Texture.h"

namespace Eng {
    namespace Loaders {
        class MeshLoader {
            static void pushVertex(Mesh::Vertex&& _vertex, Mesh::MeshData& data);
            static void processLine(const std::string& line, Mesh::MeshData& data);
        public:
            static Mesh* fromObj(Device* device, const std::string& filePath);
        };
        class TextureLoader {
        public:
            static Texture* fromBmp(Device* device, const std::string& filePath);
        };
    }
}

#endif// ENG_LAODER