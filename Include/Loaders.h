#ifndef ENG_LOADER
#define ENG_LOADER

#include "Helpers.h"
#include "Mesh.h"

namespace Eng {
    namespace Loaders {
        Mesh* meshFromObj(Device* device, const std::string& filePath);
    }
}

#endif// ENG_LAODER