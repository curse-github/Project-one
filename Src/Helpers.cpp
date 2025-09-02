#include "Helpers.h"

std::vector<char> readFile(const std::string& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("failed to open file: " + path);
    size_t size = static_cast<size_t>(file.tellg());
    std::vector<char> buff(size);
    file.seekg(0);
    file.read(buff.data(), size);
    file.close();
    return buff;
}