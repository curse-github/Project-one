#include "Loaders.h"

namespace std {
    template <>
    struct hash<Eng::Mesh::Vertex> {
        size_t operator()(Eng::Mesh::Vertex const& vertex) const {
            size_t seed = 0;
            hashCombine(seed, vertex.position, vertex.uv, vertex.normal);
            return seed;
        }
    };
}
namespace Eng {
    namespace Loaders {
        std::vector<vec3> positions;
        bool hasColors = false;
        std::vector<vec2> uvs;
        std::vector<vec3> normals;
#if defined(_DEBUG) && (_DEBUG==1)
        unsigned int numTris = 0;
#endif
        std::unordered_map<Mesh::Vertex, unsigned int> uniqueVertices{};

        std::vector<float> floats;
        std::vector<int> ints;
        std::string num = "";
        void pushVertex(Mesh::Vertex&& _vertex, Mesh::MeshData& data) {
            Mesh::Vertex vertex = _vertex;
            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<unsigned int>(data.vertices.size());
                data.vertices.push_back((Mesh::Vertex&&)vertex);
            }
            data.indices.push_back(uniqueVertices[vertex]);
        }
        void processLine(const std::string& line, Mesh::MeshData& data) {
            size_t llen = line.size();
            if (line[0] == '#') {// comments
                return;
            } else if (line[0] == 'm') {// mtllib, material file
                return;
            } else if (line[0] == 'u') {// usemtl, material to use
                return;
            } else if (line[0] == 'o') {// object name
                return;
            } else if (line[0] == 'g') {// group name
                return;
            } else if (line[0] == 'v') {// vertex data
                if (line[1] == ' ') {
                    // parse numbers
                    floats.reserve(6);
                    for (size_t j = 2; j < llen; j++) {
                        if ((line[j] == '-') || (line[j] == '.') || ((line[j] > '/') && (line[j] < ':')))// between 0-9
                            num += line[j];
                        else if (line[j] == ' ') { floats.push_back(std::stof(num)); num=""; }
                        else throw std::runtime_error("invalid wav file: invalid character found in vertex position/color definition.");
                    }
                    floats.push_back(std::stof(num)); num="";
                    // do things with floats
                    if (floats.size() == 3) {
                        if (hasColors) throw std::runtime_error("invalid wav file: only some vertices have colors.");
                        positions.push_back(vec3(floats[0], floats[1], floats[2]));
                    } else if (floats.size() == 6) {
                        if (!hasColors) {
                            if (positions.size() != 0) throw std::runtime_error("invalid wav file: only some vertices have colors.");
                            else hasColors = true;
                        }
                        positions.push_back(vec3(floats[0], floats[1], floats[2]));
                        // positions.push_back(vec3(floats[3], floats[4], floats[5]));// positions contain colors
                    } else throw std::runtime_error("invalid wav file: incorrect number of numbers in vertex position.");
                } else if (line[1] == 't') {
                    // parse numbers
                    floats.reserve(2);
                    for (size_t j = 3; j < llen; j++) {
                        if ((line[j] == '-') || (line[j] == '.') || ((line[j] > '/') && (line[j] < ':')))// between 0-9
                            num += line[j];
                        else if (line[j] == ' ') { floats.push_back(std::stof(num)); num=""; }
                        else throw std::runtime_error("invalid wav file: invalid character found in vertex uv definition.");
                    }
                    floats.push_back(std::stof(num)); num="";
                    // do things with floats
                    if (floats.size() == 2) uvs.push_back(vec2(floats[0], floats[1]));
                    else throw std::runtime_error("invalid wav file: incorrect number of numbers in vertex uv.");
                } else if (line[1] == 'n') {
                    // parse numbers
                    floats.reserve(3);
                    for (size_t j = 3; j < llen; j++) {
                        if ((line[j] == '-') || (line[j] == '.') || ((line[j] > '/') && (line[j] < ':')))// between 0-9
                            num += line[j];
                        else if (line[j] == ' ') { floats.push_back(std::stof(num)); num=""; }
                        else throw std::runtime_error("invalid wav file: invalid character found in vertex normal definition.");
                    }
                    floats.push_back(std::stof(num)); num="";
                    // do things with numbers
                    if (floats.size() == 3) normals.push_back(vec3(floats[0], floats[1], floats[2]));
                    else throw std::runtime_error("invalid wav file: incorrect number of numbers in vertex normal.");
                }
                floats.clear();
            } else if (line[0] == 's')// smooth shading on or off
                return;
            else if (line[0] == 'f') {
                // parse ints
                ints.reserve(12);
                for (size_t j = 2; j < llen; j++) {
                    if ((line[j] > '/') && (line[j] < ':')) num += line[j];// between 0-9
                    else if ((line[j] == '/') || (line[j] == ' ')) {
                        if (num != "") {
                            ints.push_back(std::stoi(num)-1); num="";
                        } else ints.push_back(-1);
                    }
                    else throw std::runtime_error("invalid wav file: invalid character found in face definition");
                }
                if (num != "") {
                    ints.push_back(std::stoi(num)-1); num="";
                } else ints.push_back(-1);
                // do things with ints
                if (ints.size() == 9) {
#if defined(_DEBUG) && (_DEBUG==1)
                    ++numTris;
#endif
                    pushVertex({
                        positions[ints[0]],
                        (ints[1]==-1)?vec2(0.0f, 0.0f):uvs[ints[1]],
                        (ints[2]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[2]]
                    }, data);
                    pushVertex({
                        positions[ints[3]],
                        (ints[4]==-1)?vec2(0.0f, 0.0f):uvs[ints[4]],
                        (ints[5]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[5]]
                    }, data);
                    pushVertex({
                        positions[ints[6]],
                        (ints[7]==-1)?vec2(0.0f, 0.0f):uvs[ints[7]],
                        (ints[8]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[8]]
                    }, data);
                } else if (ints.size() == 12) {
#if defined(_DEBUG) && (_DEBUG==1)
                    numTris += 2;
#endif
                    pushVertex({
                        positions[ints[0]],
                        (ints[1]==-1)?vec2(0.0f, 0.0f):uvs[ints[1]],
                        (ints[2]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[2]]
                    }, data);
                    pushVertex({
                        positions[ints[3]],
                        (ints[4]==-1)?vec2(0.0f, 0.0f):uvs[ints[4]],
                        (ints[5]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[5]]
                    }, data);
                    pushVertex({
                        positions[ints[6]],
                        (ints[7]==-1)?vec2(0.0f, 0.0f):uvs[ints[7]],
                        (ints[8]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[8]]
                    }, data);
                    pushVertex({
                        positions[ints[9]],
                        (ints[10]==-1)?vec2(0.0f, 0.0f):uvs[ints[10]],
                        (ints[11]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[11]]
                    }, data);
                    pushVertex({
                        positions[ints[0]],
                        (ints[1]==-1)?vec2(0.0f, 0.0f):uvs[ints[1]],
                        (ints[2]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[2]]
                    }, data);
                    pushVertex({
                        positions[ints[6]],
                        (ints[7]==-1)?vec2(0.0f, 0.0f):uvs[ints[7]],
                        (ints[8]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[8]]
                    }, data);
                } else throw std::runtime_error("invalid wav file: invalid face definition");
                ints.clear();
            }
        }
        Mesh* meshFromObj(Device* device, const std::string& filePath) {
#if defined(_DEBUG) && (_DEBUG==1)
            std::chrono::_V2::system_clock::time_point startTime = std::chrono::high_resolution_clock::now();
#endif
            std::vector<char> file = readFile(filePath);
            const size_t flen = file.size();
            std::string line;
            
            Mesh::MeshData data;
            for (size_t i = 0; i < flen; i++) {
                if (file[i] == '\n') {
                    processLine(line, data);
                    line = "";
                } else if (file[i] == '\r') continue;
                else line += file[i];
            }
            processLine(line, data);
#if defined(_DEBUG) && (_DEBUG==1)
            std::chrono::_V2::system_clock::time_point endTime = std::chrono::high_resolution_clock::now();
            float timeTook = std::chrono::duration<float, std::chrono::milliseconds::period>(endTime-startTime).count();
            std::cout << "reading file \"" << filePath << "\" took " << timeTook << "ms\n";
            std::cout << "    file contains " << data.vertices.size() << " vertices,\n";
            std::cout << "    " << data.indices.size() << " indices,\n";
            std::cout << "    and " << numTris << " tris.\n";
#endif
            positions.clear();
            hasColors = false;
            uvs.clear();
            normals.clear();
#if defined(_DEBUG) && (_DEBUG==1)
            numTris = 0;
#endif
            return new Mesh(device, data);
        }
    }
}