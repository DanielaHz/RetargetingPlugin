#include <gtest/gtest.h>
#include <iostream>
#include "FacialMesh.h"
#include <glm/vec3.hpp>

TEST(FacialMesh, ComputeBoundingBox)
{
    FacialMesh face;
    glm::vec3 minBBValue = {0.0f, 3.0f, 0.0f};
    glm::vec3 maxBBValue = {10.0f, 25.0f, 2.0f};

    // Function logic 
    glm::vec3 resultA = face.computeBoundingBox(maxBBValue, minBBValue);

    // Test logic
    float deltaX = maxBBValue.x - minBBValue.x;
    float deltaY = maxBBValue.y - minBBValue.y;
    float deltaZ = maxBBValue.z - minBBValue.z;

    glm::vec3 resultB = {deltaX, deltaY, deltaZ};

    EXPECT_EQ(resultA.x, resultB.x) << "X component mismatch. Expected: " << deltaX << ", but got: " << resultA.x;
    EXPECT_EQ(resultA.y, resultB.y) << "Y component mismatch. Expected: " << deltaY << ", but got: " << resultA.y;
    EXPECT_EQ(resultA.z, resultB.z) << "Z component mismatch. Expected: " << deltaZ << ", but got: " << resultA.z;
}

TEST(FacialMesh, getVertex)
{
    FacialMesh face;
    int index = 1;
    std::vector<glm::vec3> vertices;

    // Set up the vertices in the FacialMesh object
    face.setVertex(vertices, {1.0f, 7.0f, 3.0f});
    face.setVertex(vertices, {3.0f, 2.0f, 10.0f});
    face.setVertex(vertices, {12.0f, 13.0f, 6.0f});

    // Expected vertex
    glm::vec3  expectedVertex = {3.0f, 2.0f, 10.0f};

    // Get vertex from the object
    glm::vec3  resultVertex = face.getVertex(vertices, index);

    // Component-wise comparison with custom failure messages
    EXPECT_FLOAT_EQ(resultVertex.x, expectedVertex.x)
        << "X component mismatch at index " << index << ". Expected: " 
        << expectedVertex.x << ", but got: " << resultVertex.x;

    EXPECT_FLOAT_EQ(resultVertex.y, expectedVertex.y)
        << "Y component mismatch at index " << index << ". Expected: " 
        << expectedVertex.y << ", but got: " << resultVertex.y;

    EXPECT_FLOAT_EQ(resultVertex.z, expectedVertex.z)
        << "Z component mismatch at index " << index << ". Expected: " 
        << expectedVertex.z << ", but got: " << resultVertex.z;
}

TEST(FacialMesh, setVertex) {
    FacialMesh face;
    std::vector<glm::vec3> vertices;
    glm::vec3 vertex = {0.0f, 0.0f, 1.0f};

    face.setVertex(vertices, vertex);
    int index = 0;
    glm::vec3 result = face.getVertex(vertices, index); // Assuming index 0 if it's the first

    EXPECT_EQ(result, vertex) << "setVertex() failed to store the correct vertex at index " << index << ". "
    << "Expected: (" << vertex.x << ", " << vertex.y << ", " << vertex.z 
    << "), but got: (" << result.x << ", " << result.y << ", " << result.z << ").";
}