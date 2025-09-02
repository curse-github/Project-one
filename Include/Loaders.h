#ifndef __LOADER
#define __LOADER

#include "Helpers.h"
#include "Mesh.h"

namespace Eng {
    namespace Loaders {
        Mesh* meshFromObj(Device* device, const std::string& filePath);
    }
}

#endif// __LAODER