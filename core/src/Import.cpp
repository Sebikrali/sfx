#include "sfx/Import.hpp"

#include "sfx/Mesh.hpp"

std::vector<Object> Import::importAllMeshes(const std::string& file) {
    std::vector<Object> objects;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    if (!scene) {
        std::cerr << "Can't load model file '" << file << "'\n";
        return objects;
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>("assets/shaders/universal.vert", "assets/shaders/universal.frag");

    objects.reserve(scene->mNumMeshes);
    for (int i = 0; i < scene->mNumMeshes; i++) {
        auto mesh = std::make_shared<Mesh>(MeshData(scene->mMeshes[i]), glm::mat4(1.0f));

        // TODO: Read material and textures in (maybe even shaders)

        objects.emplace_back(scene->mMeshes[i]->mName.C_Str(), mesh, shader, nullptr, nullptr);
    }

    return objects;
}

