#include "FacialMesh.h"

glm::vec3 FacialMesh::computeBoundingBox(glm::vec3 &maxBBValue, glm::vec3 &minBBValue)
{   
    float deltaX = maxBBValue.x - minBBValue.x;
    float deltaY = maxBBValue.y - minBBValue.y;
    float deltaZ = maxBBValue.z - minBBValue.z;
    return {deltaX, deltaY , deltaZ};
}

glm::vec3 FacialMesh::getVertex(std::vector<glm::vec3> &vertices, int &index)
{
    return vertices[index];
}

void FacialMesh::setVertex(std::vector<glm::vec3> &vertices, glm::vec3 vertex)
{
    vertices.push_back(vertex);
}

std::vector<glm::vec3> FacialMesh::loadModel(const char* modelPath)
{
    std::vector<glm::vec3> meshVertices;
    
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath);

    if (!warn.empty()) std::cout << "Warning: " << warn << std::endl;
    if (!err.empty()) std::cerr << "Error: " << err << std::endl;
    if (!ret) {
        std::cerr << "Failed to load OBJ file: " << modelPath << std::endl;
        return meshVertices;
    }

    size_t numVertices = attrib.vertices.size() / 3;
    for (size_t i = 0; i < numVertices; ++i) {
        float vx = attrib.vertices[3 * i + 0];
        float vy = attrib.vertices[3 * i + 1];
        float vz = attrib.vertices[3 * i + 2];
        meshVertices.push_back({vx, vy, vz});
    }

    return meshVertices;
}
