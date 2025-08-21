#ifndef FACIALMESH_H_
#define FACIALMESH_H_

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <tiny_obj_loader.h>

/**
 * @class FacialMesh
 * @brief Provides utilities for loading and manipulating facial mesh data.
 *
 * This class supports loading OBJ models, computing bounding boxes, and accessing or modifying vertex data.
 * It is used as a foundational component in facial animation workflows.
 */
class FacialMesh {
public:
    /**
     * @brief Default constructor.
     */
    FacialMesh() = default;

    /**
     * @brief Computes the center of the bounding box defined by max and min values.
     * @param maxBBValue Maximum bounding box corner (x, y, z).
     * @param minBBValue Minimum bounding box corner (x, y, z).
     * @return Center point of the bounding box.
     */
    glm::vec3 computeBoundingBox(glm::vec3& maxBBValue, glm::vec3& minBBValue);

    /**
     * @brief Retrieves a vertex from the vertex list by index.
     * @param vertices Vector of mesh vertices.
     * @param index Index of the desired vertex.
     * @return The vertex at the specified index.
     */
    glm::vec3 getVertex(std::vector<glm::vec3>& vertices, int& index);

    /**
     * @brief Adds a vertex to the vertex list.
     * @param vertices Vector of mesh vertices.
     * @param vertex Vertex to be added.
     */
    void setVertex(std::vector<glm::vec3>& vertices, glm::vec3 vertex);

    /**
     * @brief Loads a 3D model from an OBJ file.
     * @param modelPath Path to the OBJ file.
     * @return Vector of vertex positions extracted from the model.
     */
    std::vector<glm::vec3> loadModel(const char* modelPath);
};

#endif