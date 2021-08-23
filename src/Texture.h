#pragma once

#include <string>
#include "stb_image.h"

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;  // we store the path of the texture to compare with other textures
    int width, height, nrComponents = 0;
    unsigned char* data;
    int init(const char* path, const std::string& directory) {
        std::string filename = std::string(path);
        filename = directory + "\\" + filename;
        unsigned int textureID = 0;

        data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (!data)
            std::cout << "Texture failed to load at path: " << directory + path << std::endl;
        return textureID;
    }
    int getWidth() { return width; }
    int getHeight() { return height; }
    const unsigned char* getData() { return data; }
};
