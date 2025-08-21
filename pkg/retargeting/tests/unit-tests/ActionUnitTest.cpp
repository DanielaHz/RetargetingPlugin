#include <gtest/gtest.h>
#include "Side.h"
#include "ActionUnit.h"         
#include <filesystem>
#include <glm/vec3.hpp>
#include <vector>
#include <unordered_map>

// This test unit was created to validate the values calculated as the delta transfer.
// The data was generated and saved into the deltaTransfer.json file.

TEST(ActionUnit, TestingDeltaTransferAU1_Center)
{
    ActionUnit auObject;

    const char* deltatransfer = "cmd/retargeting/data/deltaTransfer.json";
    // std::cout << "[DEBUG] the muscles path is: " << deltatransfer << "\n";

    // loading the delta transfer JSON
    auObject.loadDeltaTransfersFromJSON(deltatransfer);

    auto auDeltaTable = auObject.getAuDeltaTable();

    // get the action unit for the entry 1
    const auto& entries = auDeltaTable.at(1);
    auto it = std::find_if(
        entries.begin(), entries.end(),
        [](auto const& aud){ return aud.auId == 1 && sideToString(aud.side) == "center"; }
    );
    ASSERT_NE(it, entries.end()) << "there is not AU1 =\"center\"";
    const ActionUnitDelta& aud = *it;

    // uploading vertices of the neutral face
    const char* neutralPath = "cmd/retargeting/models/TargetTemplate.obj";
    auto neutralVerts = auObject.getVerticesNeutralFace(neutralPath);
    ASSERT_FALSE(neutralVerts.empty());

    // uploading vertices of the blendshape AU1
    const char* bsPath = "cmd/retargeting/models/Blendshapes/AU1_InnerEyebrowsRaise.obj";
    auto bsVerts = auObject.getVerticesBlendshape(bsPath);
    ASSERT_EQ(bsVerts.size(), neutralVerts.size())
        << "AU1: same number of vertices in blendhsape and neutral face";

    // comparing the data of the deltatransfer values
    for (auto const& muscleDelta : aud.activeMuscles) {
        for (auto const& vd : muscleDelta.deltas) {
          
            // expected value: blendshape - neutral
            const glm::vec3 expected = bsVerts[vd.vertexIndex]
                                     - neutralVerts[vd.vertexIndex];

            // comparing values
            EXPECT_EQ(vd.delta, expected)
                << "Mismatch in AU1, muscleId=" << muscleDelta.muscleId
                << ", vertex=" << vd.vertexIndex;
        }
    }
}

TEST(ActionUnit, TestingDeltaTransferAU5_Center)
{
    ActionUnit auObject;

    const char* deltatransfer = "cmd/retargeting/data/deltaTransfer.json";
    // std::cout << "[DEBUG] the muscles path is: " << deltatransfer << "\n";

    // loading the delta transfer JSON
    auObject.loadDeltaTransfersFromJSON(deltatransfer);

    auto auDeltaTable = auObject.getAuDeltaTable();

    // get the action unit for the entry 5
    const auto& entries = auDeltaTable.at(5);
    auto it = std::find_if(
        entries.begin(), entries.end(),
        [](auto const& aud){ return aud.auId == 5 && sideToString(aud.side) == "center"; }
    );
    ASSERT_NE(it, entries.end()) << "there is not AU5 =\"center\"";
    const ActionUnitDelta& aud = *it;

    // uploading vertices of the neutral face
    const char* neutralPath = "cmd/retargeting/models/TargetTemplate.obj";
    auto neutralVerts = auObject.getVerticesNeutralFace(neutralPath);
    ASSERT_FALSE(neutralVerts.empty());

    // uploading vertices of the blendshape AU5
    const char* bsPath = "cmd/retargeting/models/Blendshapes/AU5_UpperEyelidRaise.obj";
    auto bsVerts = auObject.getVerticesBlendshape(bsPath);
    ASSERT_EQ(bsVerts.size(), neutralVerts.size())
        << "AU1: same number of vertices in blendhsape and neutral face";

    // comparing the data of the deltatransfer values
    for (auto const& muscleDelta : aud.activeMuscles) {
        for (auto const& vd : muscleDelta.deltas) {
          
            // expected value: blendshape - neutral
            const glm::vec3 expected = bsVerts[vd.vertexIndex]
                                     - neutralVerts[vd.vertexIndex];

            // comparing values
            EXPECT_EQ(vd.delta, expected)
                << "Mismatch in AU5, muscleId=" << muscleDelta.muscleId
                << ", vertex=" << vd.vertexIndex;
        }
    }
}

TEST(ActionUnit, TestingDeltaTransferAU7_Upleft)
{
    ActionUnit auObject;

    const char* deltatransfer = "cmd/retargeting/data/deltaTransfer.json";
    // std::cout << "[DEBUG] the muscles path is: " << deltatransfer << "\n";

    // loading the delta transfer JSON
    auObject.loadDeltaTransfersFromJSON(deltatransfer);

    auto auDeltaTable = auObject.getAuDeltaTable();

    // get the action unit for the entry 7
    const auto& entries = auDeltaTable.at(7);
    auto it = std::find_if(
        entries.begin(), entries.end(),
        [](auto const& aud){ return aud.auId == 7 && sideToString(aud.side) == "upleft"; }
    );
    ASSERT_NE(it, entries.end()) << "there is not AU7 =\"upleft\"";
    const ActionUnitDelta& aud = *it;

    // uploading vertices of the neutral face
    const char* neutralPath = "cmd/retargeting/models/TargetTemplate.obj";
    auto neutralVerts = auObject.getVerticesNeutralFace(neutralPath);
    ASSERT_FALSE(neutralVerts.empty());

    // uploading vertices of the blendshape AU7
    const char* bsPath = "cmd/retargeting/models/Blendshapes/AU7_LidTightener_UpLeft.obj";
    auto bsVerts = auObject.getVerticesBlendshape(bsPath);
    ASSERT_EQ(bsVerts.size(), neutralVerts.size())
        << "AU1: same number of vertices in blendhsape and neutral face";

    // comparing the data of the deltatransfer values
    for (auto const& muscleDelta : aud.activeMuscles) {
        for (auto const& vd : muscleDelta.deltas) {
          
            // expected value: blendshape - neutral
            const glm::vec3 expected = bsVerts[vd.vertexIndex]
                                     - neutralVerts[vd.vertexIndex];

            // comparing values
            EXPECT_EQ(vd.delta, expected)
                << "Mismatch in AU7, muscleId=" << muscleDelta.muscleId
                << ", vertex=" << vd.vertexIndex;
        }
    }
}


TEST(ActionUnit, TestingDeltaTransferAU18_left)
{
    ActionUnit auObject;

    const char* deltatransfer = "cmd/retargeting/data/deltaTransfer.json";
    // std::cout << "[DEBUG] the muscles path is: " << deltatransfer << "\n";

    // loading the delta transfer JSON
    auObject.loadDeltaTransfersFromJSON(deltatransfer);

    auto auDeltaTable = auObject.getAuDeltaTable();

    // get the action unit for the entry 18
    const auto& entries = auDeltaTable.at(18);
    auto it = std::find_if(
        entries.begin(), entries.end(),
        [](auto const& aud){ return aud.auId == 18 && sideToString(aud.side) == "left"; }
    );
    ASSERT_NE(it, entries.end()) << "there is not AU18 =\"left\"";
    const ActionUnitDelta& aud = *it;

    // uploading vertices of the neutral face
    const char* neutralPath = "cmd/retargeting/models/TargetTemplate.obj";
    auto neutralVerts = auObject.getVerticesNeutralFace(neutralPath);
    ASSERT_FALSE(neutralVerts.empty());

    // uploading vertices of the blendshape AU18
    const char* bsPath = "cmd/retargeting/models/Blendshapes/AU18_LipPucker_Left.obj";
    auto bsVerts = auObject.getVerticesBlendshape(bsPath);
    ASSERT_EQ(bsVerts.size(), neutralVerts.size())
        << "AU1: same number of vertices in blendhsape and neutral face";

    // comparing the data of the deltatransfer values
    for (auto const& muscleDelta : aud.activeMuscles) {
        for (auto const& vd : muscleDelta.deltas) {
          
            // expected value: blendshape - neutral
            const glm::vec3 expected = bsVerts[vd.vertexIndex]
                                     - neutralVerts[vd.vertexIndex];

            // comparing values
            EXPECT_EQ(vd.delta, expected)
                << "Mismatch in AU18, muscleId=" << muscleDelta.muscleId
                << ", vertex=" << vd.vertexIndex;
        }
    }
}


TEST(ActionUnit, TestingDeltaTransferAU22_lifpfunnelDnRight)
{
    ActionUnit auObject;

    const char* deltatransfer = "cmd/retargeting/data/deltaTransfer.json";
    // std::cout << "[DEBUG] the muscles path is: " << deltatransfer << "\n";

    // loading the delta transfer JSON
    auObject.loadDeltaTransfersFromJSON(deltatransfer);

    auto auDeltaTable = auObject.getAuDeltaTable();

    // get the action unit for the entry 22
    const auto& entries = auDeltaTable.at(22);
    auto it = std::find_if(
        entries.begin(), entries.end(),
        [](auto const& aud){ return aud.auId == 22 && sideToString(aud.side) == "lipfunneldnright"; }
    );
    ASSERT_NE(it, entries.end()) << "there is not AU22 =\"lipfunneldnright\"";
    const ActionUnitDelta& aud = *it;

    // uploading vertices of the neutral face
    const char* neutralPath = "cmd/retargeting/models/TargetTemplate.obj";
    auto neutralVerts = auObject.getVerticesNeutralFace(neutralPath);
    ASSERT_FALSE(neutralVerts.empty());

    // uploading vertices of the blendshape AU22
    const char* bsPath = "cmd/retargeting/models/Blendshapes/AU22_LipFunnel_DnRight.obj";
    auto bsVerts = auObject.getVerticesBlendshape(bsPath);
    ASSERT_EQ(bsVerts.size(), neutralVerts.size())
        << "AU1: same number of vertices in blendhsape and neutral face";

    // comparing the data of the deltatransfer values
    for (auto const& muscleDelta : aud.activeMuscles) {
        for (auto const& vd : muscleDelta.deltas) {
          
            // expected value: blendshape - neutral
            const glm::vec3 expected = bsVerts[vd.vertexIndex]
                                     - neutralVerts[vd.vertexIndex];

            // comparing values
            EXPECT_EQ(vd.delta, expected)
                << "Mismatch in AU22, muscleId=" << muscleDelta.muscleId
                << ", vertex=" << vd.vertexIndex;
        }
    }
}

TEST(ActionUnit, TestingDeltaTransferAU26_center)
{
    ActionUnit auObject;

    const char* deltatransfer = "cmd/retargeting/data/deltaTransfer.json";
    // std::cout << "[DEBUG] the muscles path is: " << deltatransfer << "\n";

    // loading the delta transfer JSON
    auObject.loadDeltaTransfersFromJSON(deltatransfer);

    auto auDeltaTable = auObject.getAuDeltaTable();

    // get the action unit for the entry 26
    const auto& entries = auDeltaTable.at(26);
    auto it = std::find_if(
        entries.begin(), entries.end(),
        [](auto const& aud){ return aud.auId == 26 && sideToString(aud.side) == "center"; }
    );
    ASSERT_NE(it, entries.end()) << "there is not AU26 =\"center\"";
    const ActionUnitDelta& aud = *it;

    // uploading vertices of the neutral face
    const char* neutralPath = "cmd/retargeting/models/TargetTemplate.obj";
    auto neutralVerts = auObject.getVerticesNeutralFace(neutralPath);
    ASSERT_FALSE(neutralVerts.empty());

    // uploading vertices of the blendshape AU22
    const char* bsPath = "cmd/retargeting/models/Blendshapes/AU26_JawDrop.obj";
    auto bsVerts = auObject.getVerticesBlendshape(bsPath);
    ASSERT_EQ(bsVerts.size(), neutralVerts.size())
        << "AU1: same number of vertices in blendhsape and neutral face";

    // comparing the data of the deltatransfer values
    for (auto const& muscleDelta : aud.activeMuscles) {
        for (auto const& vd : muscleDelta.deltas) {
          
            // expected value: blendshape - neutral
            const glm::vec3 expected = bsVerts[vd.vertexIndex]
                                     - neutralVerts[vd.vertexIndex];

            // comparing values
            EXPECT_EQ(vd.delta, expected)
                << "Mismatch in AU1, muscleId=" << muscleDelta.muscleId
                << ", vertex=" << vd.vertexIndex;
        }
    }
}

TEST(ActionUnit, TestingDeltaTransferAU24_center)
{
    ActionUnit auObject;

    const char* deltatransfer = "cmd/retargeting/data/deltaTransfer.json";
    // std::cout << "[DEBUG] the muscles path is: " << deltatransfer << "\n";

    // loading the delta transfer JSON
    auObject.loadDeltaTransfersFromJSON(deltatransfer);

    auto auDeltaTable = auObject.getAuDeltaTable();

    // get the action unit for the entry 24
    const auto& entries = auDeltaTable.at(24);
    auto it = std::find_if(
        entries.begin(), entries.end(),
        [](auto const& aud){ return aud.auId == 24 && sideToString(aud.side) == "center"; }
    );
    ASSERT_NE(it, entries.end()) << "there is not AU24 =\"center\"";
    const ActionUnitDelta& aud = *it;

    // uploading vertices of the neutral face
    const char* neutralPath = "cmd/retargeting/models/TargetTemplate.obj";
    auto neutralVerts = auObject.getVerticesNeutralFace(neutralPath);
    ASSERT_FALSE(neutralVerts.empty());

    // uploading vertices of the blendshape AU24
    const char* bsPath = "cmd/retargeting/models/Blendshapes/AU24_LipDepressor.obj";
    auto bsVerts = auObject.getVerticesBlendshape(bsPath);
    ASSERT_EQ(bsVerts.size(), neutralVerts.size())
        << "AU1: same number of vertices in blendhsape and neutral face";

    // comparing the data of the deltatransfer values
    for (auto const& muscleDelta : aud.activeMuscles) {
        for (auto const& vd : muscleDelta.deltas) {
          
            // expected value: blendshape - neutral
            const glm::vec3 expected = bsVerts[vd.vertexIndex]
                                     - neutralVerts[vd.vertexIndex];

            // comparing values
            EXPECT_EQ(vd.delta, expected)
                << "Mismatch in AU1, muscleId=" << muscleDelta.muscleId
                << ", vertex=" << vd.vertexIndex;
        }
    }
}

TEST(ActionUnit, TestingDeltaTransferAU45_left)
{
    ActionUnit auObject;

    const char* deltatransfer = "cmd/retargeting/data/deltaTransfer.json";
    // std::cout << "[DEBUG] the muscles path is: " << deltatransfer << "\n";

    // loading the delta transfer JSON
    auObject.loadDeltaTransfersFromJSON(deltatransfer);

    auto auDeltaTable = auObject.getAuDeltaTable();

    // get the action unit for the entry 45
    const auto& entries = auDeltaTable.at(45);
    auto it = std::find_if(
        entries.begin(), entries.end(),
        [](auto const& aud){ return aud.auId == 45 && sideToString(aud.side) == "left"; }
    );
    ASSERT_NE(it, entries.end()) << "there is not AU45 =\"left\"";
    const ActionUnitDelta& aud = *it;

    // uploading vertices of the neutral face
    const char* neutralPath = "cmd/retargeting/models/TargetTemplate.obj";
    auto neutralVerts = auObject.getVerticesNeutralFace(neutralPath);
    ASSERT_FALSE(neutralVerts.empty());

    // uploading vertices of the blendshape AU45
    const char* bsPath = "cmd/retargeting/models/Blendshapes/AU45_Blink_Left.obj";
    auto bsVerts = auObject.getVerticesBlendshape(bsPath);

    ASSERT_EQ(bsVerts.size(), neutralVerts.size())
        << "AU45: same number of vertices in blendhsape and neutral face";

    // comparing the data of the deltatransfer values
    for (auto const& muscleDelta : aud.activeMuscles) {
        for (auto const& vd : muscleDelta.deltas) {
          
            // expected value: blendshape - neutral
            const glm::vec3 expected = bsVerts[vd.vertexIndex]
                                     - neutralVerts[vd.vertexIndex];

            // comparing values
            EXPECT_EQ(vd.delta, expected)
                << "Mismatch in AU45, muscleId=" << muscleDelta.muscleId
                << ", vertex=" << vd.vertexIndex;
        }
    }
}


TEST(ActionUnit, TestingDeltaTransferAU30_right)
{
    ActionUnit auObject;

    const char* deltatransfer = "cmd/retargeting/data/deltaTransfer.json";
    // std::cout << "[DEBUG] the muscles path is: " << deltatransfer << "\n";

    // loading the delta transfer JSON
    auObject.loadDeltaTransfersFromJSON(deltatransfer);

    auto auDeltaTable = auObject.getAuDeltaTable();

    // get the action unit for the entry 30
    const auto& entries = auDeltaTable.at(30);
    auto it = std::find_if(
        entries.begin(), entries.end(),
        [](auto const& aud){ return aud.auId == 30 && sideToString(aud.side) == "right"; }
    );
    ASSERT_NE(it, entries.end()) << "there is not AU30 =\"right\"";
    const ActionUnitDelta& aud = *it;

    // uploading vertices of the neutral face
    const char* neutralPath = "cmd/retargeting/models/TargetTemplate.obj";
    auto neutralVerts = auObject.getVerticesNeutralFace(neutralPath);
    ASSERT_FALSE(neutralVerts.empty());

    // uploading vertices of the blendshape AU30
    const char* bsPath = "cmd/retargeting/models/Blendshapes/AU30_JawSideway_Right.obj";
    auto bsVerts = auObject.getVerticesBlendshape(bsPath);
    ASSERT_EQ(bsVerts.size(), neutralVerts.size())
        << "AU30: same number of vertices in blendhsape and neutral face";

    // comparing the data of the deltatransfer values
    for (auto const& muscleDelta : aud.activeMuscles) {
        for (auto const& vd : muscleDelta.deltas) {
          
            // expected value: blendshape - neutral
            const glm::vec3 expected = bsVerts[vd.vertexIndex]
                                     - neutralVerts[vd.vertexIndex];

            // comparing values
            EXPECT_EQ(vd.delta, expected)
                << "Mismatch in AU30, muscleId=" << muscleDelta.muscleId
                << ", vertex=" << vd.vertexIndex;
        }
    }
}

TEST(ActionUnit, TestingDeltaTransferAU43_center)
{
    ActionUnit auObject;

    const char* deltatransfer = "cmd/retargeting/data/deltaTransfer.json";
    // std::cout << "[DEBUG] the muscles path is: " << deltatransfer << "\n";

    // loading the delta transfer JSON
    auObject.loadDeltaTransfersFromJSON(deltatransfer);

    auto auDeltaTable = auObject.getAuDeltaTable();

    // get the action unit for the entry 43
    const auto& entries = auDeltaTable.at(43);
    auto it = std::find_if(
        entries.begin(), entries.end(),
        [](auto const& aud){ return aud.auId == 43 && sideToString(aud.side) == "center"; }
    );
    ASSERT_NE(it, entries.end()) << "there is not AU43 =\"center\"";
    const ActionUnitDelta& aud = *it;

    // uploading vertices of the neutral face
    const char* neutralPath = "cmd/retargeting/models/TargetTemplate.obj";
    auto neutralVerts = auObject.getVerticesNeutralFace(neutralPath);
    ASSERT_FALSE(neutralVerts.empty());

    // uploading vertices of the blendshape AU43
    const char* bsPath = "cmd/retargeting/models/Blendshapes/AU43_EyesClosed.obj";
    auto bsVerts = auObject.getVerticesBlendshape(bsPath);
    ASSERT_EQ(bsVerts.size(), neutralVerts.size())
        << "AU43: same number of vertices in blendhsape and neutral face";

    // comparing the data of the deltatransfer values
    for (auto const& muscleDelta : aud.activeMuscles) {
        for (auto const& vd : muscleDelta.deltas) {
          
            // expected value: blendshape - neutral
            const glm::vec3 expected = bsVerts[vd.vertexIndex]
                                     - neutralVerts[vd.vertexIndex];

            // comparing values
            EXPECT_EQ(vd.delta, expected)
                << "Mismatch in AU43, muscleId=" << muscleDelta.muscleId
                << ", vertex=" << vd.vertexIndex;
        }
    }
}

