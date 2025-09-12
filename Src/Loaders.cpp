#include "Loaders.h"
#include "Engine.h"

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
        static std::vector<float> floats;
        static std::vector<int> ints;
        static std::string num = "";

#pragma region MeshLoader
        static std::vector<vec3> positions;
        static std::vector<vec2> uvs;
        static std::vector<vec3> normals;
        static std::unordered_map<Mesh::Vertex, unsigned int> uniqueVertices{};
#if defined(_DEBUG) && (_DEBUG==1)
        static unsigned int numTris = 0;
#endif
        void MeshLoader::pushVertex(Mesh::Vertex&& _vertex, Mesh::MeshData& data) {
            Mesh::Vertex vertex = _vertex;
            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<unsigned int>(data.vertices.size());
                data.vertices.push_back((Mesh::Vertex&&)vertex);
            }
            data.indices.push_back(uniqueVertices[vertex]);
        }
        void MeshLoader::processLine(const std::string& line, Mesh::MeshData& data) {
            size_t llen = line.size();
            if (line[0] == '#')// comments
                return;
            else if (line[0] == 'm') {// mtllib, material file
                return;
            } else if (line[0] == 'u') {// usemtl, material to use
                return;
            } else if (line[0] == 'o')// object name
                return;
            else if (line[0] == 'g')// group name
                return;
            else if (line[0] == 'v') {// vertex data
                if (line[1] == ' ') {
                    // parse numbers
                    floats.reserve(6);
                    for (size_t j = 2; j < llen; j++) {
                        if ((line[j] == '-') || (line[j] == '.') || ((line[j] > '/') && (line[j] < ':')))// between 0-9
                            num += line[j];
                        else if (line[j] == ' ') { floats.push_back(std::stof(num)); num=""; }
                        else throw std::runtime_error("Invalid wav file: invalid character found in vertex position/color definition.");
                    }
                    floats.push_back(std::stof(num)); num="";
                    // do things with floats
                    if (floats.size() == 3) {
                        positions.push_back(vec3(floats[0], floats[1], floats[2]));
                        // positions.push_back(vec3(0.8f, 0.8f, 0.8f));// position did not contain colors
                    } else if (floats.size() == 6) {
                        positions.push_back(vec3(floats[0], floats[1], floats[2]));
                        // positions.push_back(vec3(floats[3], floats[4], floats[5]));// position contained colors
                    } else throw std::runtime_error("Invalid wav file: incorrect number of numbers in vertex position.");
                } else if (line[1] == 't') {
                    // parse numbers
                    floats.reserve(2);
                    for (size_t j = 3; j < llen; j++) {
                        if ((line[j] == '-') || (line[j] == '.') || ((line[j] > '/') && (line[j] < ':')))// between 0-9
                            num += line[j];
                        else if (line[j] == ' ') { floats.push_back(std::stof(num)); num=""; }
                        else throw std::runtime_error("Invalid wav file: invalid character found in vertex uv definition.");
                    }
                    floats.push_back(std::stof(num)); num="";
                    // do things with floats
                    if (floats.size() == 2) uvs.push_back(vec2(floats[0], floats[1]));
                    else throw std::runtime_error("Invalid wav file: incorrect number of numbers in vertex uv.");
                } else if (line[1] == 'n') {
                    // parse numbers
                    floats.reserve(3);
                    for (size_t j = 3; j < llen; j++) {
                        if ((line[j] == '-') || (line[j] == '.') || ((line[j] > '/') && (line[j] < ':')))// between 0-9
                            num += line[j];
                        else if (line[j] == ' ') { floats.push_back(std::stof(num)); num=""; }
                        else throw std::runtime_error("Invalid wav file: invalid character found in vertex normal definition.");
                    }
                    floats.push_back(std::stof(num)); num="";
                    // do things with numbers
                    if (floats.size() == 3) normals.push_back(vec3(floats[0], floats[1], floats[2]));
                    else throw std::runtime_error("Invalid wav file: incorrect number of numbers in vertex normal.");
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
                    else throw std::runtime_error("Invalid wav file: invalid character found in face definition");
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
                        (ints[2]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[2]],
                        {0.0f, 0.0f ,0.0f}
                    }, data);
                    pushVertex({
                        positions[ints[3]],
                        (ints[4]==-1)?vec2(0.0f, 0.0f):uvs[ints[4]],
                        (ints[5]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[5]],
                        {0.0f, 0.0f ,0.0f}
                    }, data);
                    pushVertex({
                        positions[ints[6]],
                        (ints[7]==-1)?vec2(0.0f, 0.0f):uvs[ints[7]],
                        (ints[8]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[8]],
                        {0.0f, 0.0f ,0.0f}
                    }, data);
                } else if (ints.size() == 12) {
#if defined(_DEBUG) && (_DEBUG==1)
                    numTris += 2;
#endif
                    pushVertex({
                        positions[ints[0]],
                        (ints[1]==-1)?vec2(0.0f, 0.0f):uvs[ints[1]],
                        (ints[2]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[2]],
                        {0.0f, 0.0f ,0.0f}
                    }, data);
                    pushVertex({
                        positions[ints[3]],
                        (ints[4]==-1)?vec2(0.0f, 0.0f):uvs[ints[4]],
                        (ints[5]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[5]],
                        {0.0f, 0.0f ,0.0f}
                    }, data);
                    pushVertex({
                        positions[ints[6]],
                        (ints[7]==-1)?vec2(0.0f, 0.0f):uvs[ints[7]],
                        (ints[8]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[8]],
                        {0.0f, 0.0f ,0.0f}
                    }, data);
                    pushVertex({
                        positions[ints[9]],
                        (ints[10]==-1)?vec2(0.0f, 0.0f):uvs[ints[10]],
                        (ints[11]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[11]],
                        {0.0f, 0.0f ,0.0f}
                    }, data);
                    pushVertex({
                        positions[ints[0]],
                        (ints[1]==-1)?vec2(0.0f, 0.0f):uvs[ints[1]],
                        (ints[2]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[2]],
                        {0.0f, 0.0f ,0.0f}
                    }, data);
                    pushVertex({
                        positions[ints[6]],
                        (ints[7]==-1)?vec2(0.0f, 0.0f):uvs[ints[7]],
                        (ints[8]==-1)?vec3(0.0f, 1.0f, 0.0f):normals[ints[8]],
                        {0.0f, 0.0f ,0.0f}
                    }, data);
                } else throw std::runtime_error("Invalid wav file: invalid face definition");
                ints.clear();
            }
        }
        Mesh* MeshLoader::fromObj(Device* device, const std::string& filePath) {
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
            uvs.clear();
            normals.clear();
#if defined(_DEBUG) && (_DEBUG==1)
            numTris = 0;
#endif
            return new Mesh(device, data);
        }
#pragma endregion MeshLoader

#pragma region TextureLoader
    Texture* TextureLoader::fromBmp(Device* device, const std::string& filePath) {
            std::vector<unsigned char> file = readFileBytes(filePath);
            const size_t flen = file.size();
            size_t i = 0;
            if ((file[0] != 'B') || (file[1] != 'M'))
                throw std::runtime_error("File is not .bmp file!");
            i = 2+4+4;// skip signature("BM"), file size, reserved bits
            unsigned int dataOffset = file[i] | (file[i+1] << 8u) | (file[i+2] << 16u) | (file[i+3] << 24u); i += 4;
            i += 4;// skip infoHeader size
            int width = file[i] | (file[i+1] << 8u) | (file[i+2] << 16u) | (file[i+3] << 24u); i += 4;
            int height = file[i] | (file[i+1] << 8u) | (file[i+2] << 16u) | (file[i+3] << 24u); i += 4;
            unsigned int planes =  file[i] | (file[i+1] << 8u); i += 2;
            if (planes != 1)
                throw std::runtime_error("Invalid bmp file: invalid number of planes!");
            unsigned int bpp = file[i] | (file[i+1] << 8u); i += 2;
            if ((bpp != 4) && (bpp != 8) && (bpp != 24) && (bpp != 32))// rgb or rgba without palleting
                throw std::runtime_error("Invalid bmp file: cannot currently handle palleted images.");
            unsigned int compression = file[i] | (file[i+1] << 8u) | (file[i+2] << 16u) | (file[i+3] << 24u); i += 4;
            if ((compression != 0) && ((compression != 3) || (bpp != 32)))// rgb or rgba
                throw std::runtime_error("Invalid bmp file: cannot currently handle compressed images.");
            bool isPalleted = (bpp == 4);
            i += 4+4+4;
            unsigned int palleteColorCount = file[i] | (file[i+1] << 8u) | (file[i+2] << 16u) | (file[i+3] << 24u); i += 4;
            i = dataOffset;// skip to data
            std::vector<unsigned char> pixels;
            if (palleteColorCount == 0) {
                for (size_t j = 0; j < width*height; j++) {
                    pixels.push_back(file[i+2u]);
                    pixels.push_back(file[i+1u]);
                    pixels.push_back(file[i+0u]);
                    if (bpp == 32) {
                        pixels.push_back(file[i+3u]);
                        i+=4;
                    } else {// bbp = 24
                        pixels.push_back(255);
                        i+=3;
                    }
                }
            } else {
                size_t palleteSize = palleteColorCount*4;
                std::vector<unsigned char> pallete(&file[i]-palleteSize, &file[i]);
                size_t padding = (4-((width/2)%4))%4;
                for (size_t j = 0; j < height; j++) {
                    bool isFirst = true;
                    for (size_t k = 0; k < width; k++) {
                        unsigned int index;
                        if (bpp == 4) {
                            if (isFirst) index = (file[i] & 0xF0) >> 4;
                            else {
                                index = file[i] & 0x0F;
                                i++;
                            }
                            isFirst=!isFirst;
                        } else if (bpp == 8) {
                            index = file[i];
                            i++;
                        }
                        pixels.push_back(pallete[index*4u+2u]);
                        pixels.push_back(pallete[index*4u+1u]);
                        pixels.push_back(pallete[index*4u+0u]);
                        pixels.push_back(255);
                    }
                    i += padding;
                }
            }
            return new Texture(device, width, height, pixels.data());
        }
#pragma endregion TextureLoader

#pragma region MaterialLoader
        std::string currMatName = "";
        MaterialUboData material{};
        void MaterialLoader::processLine(const std::string& filePath, const std::string& line, Mesh::MeshData& data, Engine* engine) {
            size_t llen = line.size();
            if ((line.size() == 0) || (line[0] == '#'))// comments
                return;
            else if ((line[0] == 'n') && (line[3] == 'm')) {// newmtl, norm
                if (line[1] == 'e') {// newmtl: name of next material
                    if ((line[2] != 'w') || (line[4] != 't') || (line[5] != 'l') || (line[6] != ' ')) throw std::runtime_error("Invalid mtl file: unknown property0");
                    if (currMatName != "")
                        engine->storeMaterial(currMatName, material);// store last material
                    currMatName = filePath+line.substr(7);// cut off "newmtl "
                    material = MaterialUboData{};
                } else if ((line[1] == 'o') && (line[2] == 'r') && (line[4] == ' ')) {// norm: normal map
                    unsigned int normalMap = engine->storeTexture(line.substr(5));// cut off "norm " and load the texture
                    material.map_norm = normalMap;
                }
                return;
            } else if (line[0] == 'K') {// Ka, Kd, Ks, Ke
                if (line[2] != ' ') throw std::runtime_error("Invalid mtl file: unknown property1");
                // for Ka, Kd, Ks, and Ke I need to parse 3 floats
                floats.reserve(3);
                for (size_t j = 3; j < llen; j++) {
                    if ((line[j] == '.') || ((line[j] > '/') && (line[j] < ':')))// between 0-9
                        num += line[j];
                    else if (line[j] == ' ') { floats.push_back(std::stof(num)); num=""; }
                    else throw std::runtime_error("Invalid mtl file: invalid character found in color.");
                }
                floats.push_back(std::stof(num)); num="";
                if (floats.size() != 3) throw std::runtime_error("Invalid mtl file: incorrect amount of numbers in color");

                if (line[1] == 'a') {// Ka: ambient color// currently treated the same as diffuse color
                    // set diffuse color while maintaining alpha
                    material.diffuseColor_Transparency = vec4(floats[0], floats[1], floats[2], material.diffuseColor_Transparency.w);
                } else if (line[1] == 'd') {// Kd: diffuse color
                    // set diffuse color while maintaining alpha
                    material.diffuseColor_Transparency = vec4(floats[0], floats[1], floats[2], material.diffuseColor_Transparency.w);
                } else if (line[1] == 's') {// Ks: specular color
                    // set specular color while maintaining exponent
                    material.specColor_Exp = vec4(floats[0], floats[1], floats[2], material.specColor_Exp.w);
                } else if (line[1] == 'e') {// Ke: emission color
                    // not currently handled
                } else {
                    throw std::runtime_error("Invalid mtl file: unknown property2");
                }
                floats.clear();
                return;
            } else if ((line[0] == 'N')) {// Ns
                if ((line[1] != 's') || (line[2] != ' ')) throw std::runtime_error("Invalid mtl file: unknown property3");
                floats.reserve(1);
                for (size_t j = 3; j < llen; j++) {
                    if ((line[j] == '.') || ((line[j] > '/') && (line[j] < ':')))// between 0-9
                        num += line[j];
                    else if (line[j] == ' ') { floats.push_back(std::stof(num)); num=""; }
                    else throw std::runtime_error("Invalid mtl file: invalid character found in color.");
                }
                floats.push_back(std::stof(num)); num="";
                if (floats.size() != 1) throw std::runtime_error("Invalid mtl file: incorrect amount of numbers in transparency");

                material.specColor_Exp.w = floats[0];
                floats.clear();
                return;
            } else if (line[0] == 'd') {// d
                if (line[1] != ' ') throw std::runtime_error("Invalid mtl file: unknown property4");
                floats.reserve(1);
                for (size_t j = 2; j < llen; j++) {
                    if ((line[j] == '.') || ((line[j] > '/') && (line[j] < ':')))// between 0-9
                        num += line[j];
                    else if (line[j] == ' ') { floats.push_back(std::stof(num)); num=""; }
                    else throw std::runtime_error("Invalid mtl file: invalid character found in color.");
                }
                floats.push_back(std::stof(num)); num="";
                if (floats.size() != 1) throw std::runtime_error("Invalid mtl file: incorrect amount of numbers in transparency");

                material.diffuseColor_Transparency.w = floats[0];
                floats.clear();
                return;
            } else if (line[0] == 'm') {// map_*
                if ((line[1] != 'a') || (line[2] != 'p') || (line[3] != '_')) throw std::runtime_error("Invalid mtl file: unknown property5");
                if (line[4] == 'K') {
                    if (line[6] != ' ') throw std::runtime_error("Invalid mtl file: unknown property6");
                    if (line[5] == 'a') {// map_Ka: ambient color map// currently treated the same as diffuse color
                        unsigned int diffuseMap = engine->storeTexture(line.substr(7));// cut off "map_Ka " and load the texture
                        material.map_diff = diffuseMap;
                        return;
                    } else if (line[5] == 'd') {// map_Kd: diffuse color map
                        unsigned int diffuseMap = engine->storeTexture(line.substr(7));// cut off "map_Kd " and load the texture
                        material.map_diff = diffuseMap;
                        return;
                    } else if (line[5] == 's') {// map_Ks: specular color map
                        unsigned int specularColorMap = engine->storeTexture(line.substr(7));// cut off "map_Ks " and load the texture
                        material.map_specC = specularColorMap;
                        return;
                    }
                    // others that arent supported
                } else if (line[4] == 'N') {// map_Ns: specular exponent map
                    std::cout << line << '\n';
                    if ((line[5] != 's') || (line[6] != ' ')) throw std::runtime_error("Invalid mtl file: unknown property7");
                    unsigned int specularExpMap = engine->storeTexture(line.substr(7));// cut off "map_Ns " and load the texture
                    material.map_specE = specularExpMap;
                    return;
                } else if ((line[4] == 'd')) {// map_d: dissolve/transparency map
                    if (line[5] != ' ') throw std::runtime_error("Invalid mtl file: unknown property8");
                    // not currently handled
                    return;
                } else if ((line[4] == 'n')) {// map_norm: normal map
                    unsigned int normalMap = engine->storeTexture(line.substr(9));// cut off "map_norm " and load the texture
                    material.map_norm = normalMap;
                    return;
                } else if ((line[4] == 'b')) {// map_bump: bump map// currently trated the same as normal map
                    unsigned int normalMap = engine->storeTexture(line.substr(9));// cut off "map_bump " and load the texture
                    material.map_norm = normalMap;
                    return;
                }
                throw std::runtime_error("Invalid mtl file: unknown property9");
            } else if (line[0] == 'b') {// bump, currently treated the same as normal map
                if (
                    (line[1] != 'u') || (line[2] != 'm') ||
                    (line[3] != 'p') || (line[4] != '_')
                ) throw std::runtime_error("Invalid mtl file: unknown property10");

                unsigned int normalMap = engine->storeTexture(line.substr(5));// cut off "bump " and load the texture
                material.map_norm = normalMap;
                return;
            } else if (line[0] == 'T') {// Tr, Tf
                return;// not handled right now
            } else if (line[0] == 'i') {
                return;// not handled right now
            } else if (line[0] == 'P') {// PBR values: Pr,Pm,Ps,Pc,Pcr
                return;// not handled right now
            } else if (line[0] == 'a') {// aniso, anisor
                return;// not handled right now
            }
            std::cout << line << '\n';
            throw std::runtime_error("Invalid mtl file: unknown property11");
        }
        void MaterialLoader::fromMtl(Device* device, const std::string& filePath, Engine* engine) {
#if defined(_DEBUG) && (_DEBUG==1)
            std::chrono::_V2::system_clock::time_point startTime = std::chrono::high_resolution_clock::now();
#endif
            std::vector<char> file = readFile(filePath);
            const size_t flen = file.size();
            std::string line;
            
            Mesh::MeshData data;
            for (size_t i = 0; i < flen; i++) {
                if (file[i] == '\n') {
                    processLine(filePath, line, data, engine);
                    line = "";
                } else if (file[i] == '\r') continue;
                else line += file[i];
            }
            processLine(filePath, line, data, engine);
            if (currMatName != "")
                engine->storeMaterial(currMatName, material);// store last material
            currMatName = "";
        }
#pragma endregion MaterialLoader
    }
}