#include "MathUtils.h"

glm::vec3 MathUtils::calculateDeltaTransfer(glm::vec3& blendshapeVertex, glm::vec3& neutralVertex)
{
    float deltaX = blendshapeVertex.x - neutralVertex.x ;
    float deltaY = blendshapeVertex.y - neutralVertex.y;
    float deltaZ = blendshapeVertex.z - neutralVertex.z;
    return {deltaX, deltaY, deltaZ};
}

float MathUtils::calculateEuclidianDistance(glm::vec3& landmarkVertexA, glm::vec3& landmarkVertexB)
{
    float result = glm::distance(landmarkVertexA, landmarkVertexB); 
    return result;
}

float MathUtils::calculateIntensity(float& thresholdMin, float& thresholdMax, float& currentDistance, float& baseDistance)
{
    float delta = currentDistance - baseDistance;
    return glm::clamp((delta - thresholdMin) / (thresholdMax - thresholdMin), 0.0f, 1.0f);
}

glm::vec3 MathUtils::calculateActivation(glm::vec3& deltaTransfer, float& intensity)
{
    return deltaTransfer*intensity;
}