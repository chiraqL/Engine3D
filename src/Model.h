#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/color4.h>

#include "Mesh.h"

extern const std::string _SOLUTIONDIR;
const std::string _OBJPATH = _SOLUTIONDIR + "resources\\model\\";
const std::string _TEXPATH = _SOLUTIONDIR + "resources\\texture\\";

class Model
{
public:
    std::vector<Mesh> meshes;       // meshes separated by materials associated
    std::string directory;
    vec3 position, rotAngle, scaleFactor;
    Model(const std::string& path, Light* light)
    {
        position = { 0.0f,0.0f,-6.0f };     // translate model away from camera at origin
        rotAngle = { 0.0f, 0.0f, 0.0f };
        scaleFactor = { 1,1,1 };
        loadModel(_OBJPATH + path);

        // Add model related lights
        PLight pl = { {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f,0.0f,5.0f}, 1 };
        light->plights.push_back(pl);
    }
    void draw(Camera* camera, Light* light, mat4 mv, mat4 p) {
        for (auto& mesh : this->meshes)
            mesh.draw(camera, light, mv, p);
    }
private:
    std::vector<Texture> textures_loaded;
    void loadModel(std::string path) {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('\\\\'));

        processNode(scene->mRootNode, scene);
    }
    void processNode(aiNode* node, const aiScene* scene) {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }
    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Tri>            tris;
        std::vector<Texture>        textures;
        col3                        ka{}, kd{}, ks{};
        float                         ns{};

        for (unsigned int i = 0; i < mesh->mNumVertices; i = i + 3)
        {
            vert vertex[3];
            for (int j = 0; j < 3; j++)
            {
                // process vertex positions, normals and texture coordinates
                vec3 vector{};
                vector.x = mesh->mVertices[i + j].x;
                vector.y = mesh->mVertices[i + j].y;
                vector.z = mesh->mVertices[i + j].z;
                vertex[j].position = vector;
                vector.x = mesh->mNormals[i + j].x;
                vector.y = mesh->mNormals[i + j].y;
                vector.z = mesh->mNormals[i + j].z;
                vertex[j].normal = vector;
                if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i + j].x;
                    vec.y = mesh->mTextureCoords[0][i + j].y;
                    vertex[j].tex = vec;
                }
                else
                    vertex[j].tex = tex2(0.0f, 0.0f);
            }
            tris.push_back({vertex[0], vertex[1] ,vertex[2]});
        }
        // process material
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            aiColor4D ambient, diffuse, specular;
            aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient);//ka
            aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);//kd
            aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);//ks
            aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &ns); //ns
            ka = { ambient.r, ambient.g , ambient.b };
            kd = { diffuse.r, diffuse.g , diffuse.b };
            ks = { specular.r, specular.g , specular.b };

            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(tris, textures, ka, kd, ks, ns);
    }
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = texture.init(str.C_Str(), _TEXPATH);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture); // add to loaded textures
            }
        }
        return textures;
    }
};