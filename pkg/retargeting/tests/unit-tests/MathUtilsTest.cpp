#include <gtest/gtest.h>
#include "MathUtils.h"
#include <glm/vec3.hpp>

TEST(MathUtils, calculateDeltaTransferTest)
{
    glm::vec3 neutralVertex = {1.0f, -5.0f, 2.0f};
    glm::vec3 blendshapeVertex = {-2.0f, 1.0f, 2.0f};

    MathUtils math;

    glm::vec3 resultA = math.calculateDeltaTransfer(blendshapeVertex, neutralVertex);
    glm::vec3 resultB = blendshapeVertex - neutralVertex;

    EXPECT_EQ(resultA, resultB);
}

TEST(MathUtils, calculateEuclidianDistance)
{
    glm::vec3 a(1.0f, 2.0f, 3.0f);
    glm::vec3 b(4.0f, 6.0f, 3.0f);

    float expected = 5.0f;

    float result = glm::distance(a, b);

    EXPECT_FLOAT_EQ(result, expected);
}