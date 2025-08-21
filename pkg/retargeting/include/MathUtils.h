#ifndef MATHUTILS_H_
#define MATHUTILS_H_

#include <vector>
#include <array>
#include <map>
#include <memory>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

/**
 * @class MathUtils
 * @brief Provides mathematical utilities for facial mesh processing and animation.
 *
 * This class handles core calculations such as delta transfers, range of motion, landmark distances,
 * intensity estimation, and coordinate space transformations between 2D video and 3D model space.
 */
class MathUtils {
public:
    /**
     * @brief Calculates the delta transfer between a blendshape vertex and its neutral counterpart.
     * @param blendshapeVertex Vertex from the blendshape mesh.
     * @param neutralVertex Corresponding vertex from the neutral mesh.
     * @return Displacement vector representing the delta.
     */
    glm::vec3 calculateDeltaTransfer(glm::vec3& blendshapeVertex, glm::vec3& neutralVertex);

    /**
     * @brief Calculates the range of motion for a landmark or vertex.
     * @return Scalar value representing the motion range.
     */
    float calculateRangeofMotion();

    /**
     * @brief Calculates the Euclidean distance between two landmark vertices.
     * @param landmarkVertexA First landmark vertex.
     * @param landmarkVertexB Second landmark vertex.
     * @return Distance between the two points.
     */
    float calculateEuclidianDistance(glm::vec3& landmarkVertexA, glm::vec3& landmarkVertexB);

    /**
     * @brief Calculates the intensity of an Action Unit activation based on landmark displacement.
     * @return Intensity value in the range [0.0, 1.0].
     */
    float calculateIntensity(float& thresholdMin, float& thresholdMax, float& currentDistance, float& baseDistance);

    /**
     * @brief Calculates the activation of an passive and active muscle vertices based on the intensity of activation
     * @return Activation value
     */
    glm::vec3 calculateActivation(glm::vec3& deltaTransfer, float& intensity);
};

#endif