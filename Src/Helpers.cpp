#include "Helpers.h"

std::vector<char> readFile(const std::string& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + path);
    size_t size = static_cast<size_t>(file.tellg());
    std::vector<char> buff(size);
    file.seekg(0);
    file.read(buff.data(), size);
    file.close();
    return buff;
}
std::vector<unsigned char> readFileBytes(const std::string& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + path);
    size_t size = static_cast<size_t>(file.tellg());
    std::vector<unsigned char> buff(size);
    file.seekg(0);
    file.read((char*)buff.data(), size);
    file.close();
    return buff;
}