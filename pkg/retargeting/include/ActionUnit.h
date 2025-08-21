#ifndef ACTIONUNIT_H_
#define ACTIONUNIT_H_

#include <unordered_map>
#include <vector>
#include <glm/vec3.hpp>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>

#include "FacialMesh.h"
#include "MathUtils.h"
#include "Side.h"

/**
 * @struct VertexDelta
 * @brief Represents the delta deformation applied to a single vertex of a muscle.
 */
struct VertexDelta {
    int vertexIndex;       ///< Index of the vertex in the mesh
    glm::vec3 position;    ///< Original position of the vertex
    glm::vec3 delta;       ///< Displacement vector applied when the muscle activates (passively and actively muscles)
};

/**
 * @struct MuscleDelta
 * @brief Represents all vertex deltas associated with a single muscle.
 */

struct MuscleDelta {
    int muscleId;                      ///< Identifier of the muscle
    std::vector<VertexDelta> deltas;   ///< List of vertex deltas for this muscle
};

/**
 * @struct ActionUnitDelta
 * @brief Represents deformation data for a single Action Unit (AU).
 *
 * Includes both active and passive muscles contributing to the AU's effect.
 */

struct ActionUnitDelta {
    int auId;                          ///< Action Unit identifier
    Side side;                  ///< Side of the face (e.g., "left", "right", "center")
    std::vector<MuscleDelta> activeMuscles;   ///< Muscles actively contributing to the AU
    std::vector<MuscleDelta> passiveMuscles;  ///< Muscles passively affected by the AU
};

/**
 * @class ActionUnit
 * @brief Manages facial deformation data based on Action Units (AUs).
 *
 * This class loads mesh data, muscle mappings, and delta transfers from JSON files.
 * It calculates and applies deformations to mesh vertices based on AU activation.
 */
class ActionUnit {
public:
    /**
     * @brief Constructs the ActionUnit and initializes internal components.
     */
    ActionUnit () {
        m_facialMesh = std::make_unique<FacialMesh>();
        m_mathUtils = std::make_unique<MathUtils>();
    };

    /**
     * @brief Loads model paths from a JSON configuration file.
     * @param pathsJson JSON string containing model paths.
     * @param basePath Base directory for resolving relative paths.
     * @return True if loading was successful.
     */
    bool loadModelPathsFromJSON(const char* pathsJson, const char* basePath);

    /**
     * @brief Loads the muscle index map from a JSON file.
     * @param musclesJson JSON string containing muscle-to-vertex mappings.
     * @return True if parsing was successful.
     */
    bool loadMuscleIndexMapFromJSON(const char* musclesJson);

    /**
     * @brief Loads precomputed delta transfer data from a JSON file.
     * @param deltaJson JSON string containing AU deformation data.
     */
    void loadDeltaTransfersFromJSON(const char* deltaJson);

    /**
     * @brief Retrieves the vertices of the neutral (rest) face mesh.
     * @param modelPath Path to the neutral face model.
     * @return Vector of vertex positions.
     */
    std::vector<glm::vec3> getVerticesNeutralFace(const char* modelPath);

    /**
     * @brief Retrieves the vertices of a specific blendshape mesh.
     * @param modelPath Path to the blendshape model.
     * @return Vector of vertex positions.
     */
    std::vector<glm::vec3> getVerticesBlendshape(const char* modelPath);

    /**
     * @brief Computes muscle vertex deltas between neutral and blendshape meshes.
     * @param neutralFaceVertices Vertices of the neutral face.
     * @param blendshapeVertices Vertices of the blendshape.
     * @param activeMuscleList List of active muscle IDs.
     * @return Vector of MuscleDelta structures.
     */
    std::vector<MuscleDelta> getMusclesVertices(
        const std::vector<glm::vec3>& neutralFaceVertices,
        const std::vector<glm::vec3>& blendshapeVertices,
        const std::vector<int>& activeMuscleList);

    /**
     * @brief Populates the muscle index map from parsed JSON data.
     * @param data Parsed JSON object.
     * @return Muscle index map.
     */
    std::unordered_map<int, std::vector<int>> populateMuscleIndexMap(nlohmann::json& data);

    /**
     * @brief Prints the muscle index map for debugging.
     */
    void printMuscleIndexMap();

    /**
     * @brief Prints a vector of vertex positions for debugging.
     * @param vector Vector of glm::vec3 positions.
     */
    void printVector(std::vector<glm::vec3>& vector);

    /**
     * @brief Parses an Action Unit ID from a string key.
     * @param auKey String representation of the AU.
     * @return Parsed integer ID.
     */
    static int parseAUId(const std::string& auKey);

    /**
     * @brief Saves the delta transfer data to a JSON file.
     * @param outJsonPath Path to the output JSON file.
     * @return True if saving was successful.
     */
    bool saveDeltaTransfersToJSON(const char* outJsonPath);

    /**
     * @brief Prints the AU delta table for inspection.
     */
    void printauDeltaTable();

    /**
     * @brief Applies the deformation of an Action Unit to a mesh.
     * @param auDelta ActionUnitDelta containing deformation data.
     * @param meshVertices Mesh vertices to be modified.
     */

    /**
     * @brief Returns the AU delta table.
     * @return Map of AU IDs to their corresponding delta data.
     */
    std::unordered_map<int, std::vector<ActionUnitDelta>> getAuDeltaTable();

    /**
     * @brief Returns the muscle index map.
     * @return Map of muscle IDs to vertex indices.
     */
    std::unordered_map<int, std::vector<int>> getMuscleIndexMap();

private:
    std::unique_ptr<FacialMesh> m_facialMesh;                             ///< Facial mesh utility for loading and processing mesh data
    std::unique_ptr<MathUtils> m_mathUtils;                               ///< Utility for computing delta transfers
    std::unordered_map<int, std::vector<ActionUnitDelta>> m_auDeltaTable; ///< AU deformation data
    std::unordered_map<int, std::vector<int>> m_muscleIndexMap;           ///< Muscle-to-vertex mapping
    std::vector<glm::vec3> m_neutralFaceVertices;                         ///< Cached neutral face vertices
    VertexDelta m_vertexDelta;                                            ///< Temporary vertex delta container
};

#endif 
