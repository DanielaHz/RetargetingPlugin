#include <maya/MFileIO.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDagNode.h>
#include <QByteArray>
#include <maya/MFnTransform.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MStringArray.h>
#include <maya/MDagModifier.h>
#include "MayaMesh.h"
#include <maya/MVector.h>

MString MayaMesh::convertModelPathToMString(const QString& path)
{
    QString cleaned = path;
    if (cleaned.startsWith('\"') && cleaned.endsWith('\"')) {
        cleaned = cleaned.mid(1, cleaned.length() - 2);
    }
    QByteArray utf8 = cleaned.toUtf8();
    return MString(utf8.constData());
}

MStatus MayaMesh::importObjMesh(const MString& objPath, MObject& transformOut, MObject& shapeOut, std::string& name)
{
    MStringArray newNodes;
    MString cmd =
        "file -import "
        "-type \"OBJ\" "
        "-ignoreVersion "
        "-options \"mo=1\" "
        "-pr "
        "-rnn \"" + objPath + "\";";
    MStatus status = MGlobal::executeCommand(cmd, newNodes);
    if (status != MS::kSuccess || newNodes.length() < 2) {
        MGlobal::displayError("Failed to import OBJ: " + objPath);
        return MS::kFailure;
    }

    MSelectionList sel;
    sel.add(newNodes[0]);
    sel.add(newNodes[1]);
    sel.getDependNode(0, transformOut);
    sel.getDependNode(1, shapeOut);

    // Creating the muscle mesh
    MString meshName(name.c_str());
    MStatus st = renameMesh(transformOut, meshName);
    if (st != MS::kSuccess) {
        MGlobal::displayError("Failed to rename transform to " + meshName);
        return st;
    }

    return MS::kSuccess;
}

MStatus MayaMesh::loadMayaMuscle(const MString& objPath)
{
    std::string _name = "Muscle";
    MStatus status = importObjMesh(objPath, _muscleTransform, _muscleShape, _name);
    if (status != MS::kSuccess)
        MGlobal::displayError("Error importing OBJ: " + objPath);

    return status;
}

MStatus MayaMesh::loadMayaSkin(const MString& objPath)
{
    std::string _name = "Skin";

    MStatus status = importObjMesh(objPath, _skinTransform, _skinShape, _name);
    if (status != MS::kSuccess)
        MGlobal::displayError("Error importing OBJ: " + objPath);

    MFnTransform fnTransform(_skinTransform, &status);
    if (status == MS::kSuccess) {
        double scale[3] = {1.01, 1.01, 1.01}; // 1% scale
        fnTransform.setScale(scale);
    }

    MVector offset(0.0, 0.0, 0.2); // to avoid overlapping between meshes
    fnTransform.setTranslation(offset, MSpace::kTransform);
    return status;
}

MStatus MayaMesh::loadMayaSkull(const MString& objPath)
{
    std::string _name = "Skull";

    MStatus status = importObjMesh(objPath, _skullTransform, _skullShape, _name);
    if (status != MS::kSuccess)
        MGlobal::displayError("Error importing OBJ: " + objPath);

    return status;
}

MStatus MayaMesh::positionTransform(const MObject& transform, const MVector& position, MSpace::Space space)
{
    MFnTransform fn(transform);
    fn.setTranslation(position, space);
    return MS::kSuccess;
}

MStatus MayaMesh::renameMesh(const MObject& transform, const MString& newName)
{
    MFnDagNode fn(transform);
    fn.setName(newName);
    return MS::kSuccess;
}

MObject MayaMesh::getMayaMuscle()
{
    return _muscleShape;
}

MObject MayaMesh::createJoint(const MVector& position, const std::string& name) 
{
    MStatus status;
    MFnIkJoint jointFn;
    MObject joint = jointFn.create(MObject::kNullObj, &status);
    if (status == MS::kSuccess) {
        jointFn.setTranslation(position, MSpace::kTransform);
        jointFn.setName(name.c_str());
    } else {
        MGlobal::displayError("Failed to create joint.");
    }
    return joint;
}

MStatus MayaMesh::prepareMeshSkinning(std::vector<glm::vec3> m_inputMeshLandmarks3D)
{
    std::cout << "[MAYAMESH] The size of the input Mesh Landmarks 3D is: " << m_inputMeshLandmarks3D.size() << " landmarks entries.\n";

    MObject muscle = getMayaMuscle();
    if (muscle.isNull()) {
        MGlobal::displayError("Muscle mesh is invalid.");
        return MS::kFailure;
    }

    MObjectArray jointObjects;

    std::cout << " [MAYAMESH] ------The joints positions in the 3D Input mesh are: ------" << "\n";
    for (size_t i = 0; i < m_inputMeshLandmarks3D.size(); ++i)
    {
        const glm::vec3& landmark = m_inputMeshLandmarks3D[i];
        MVector jointPos(landmark.x, landmark.y, landmark.z);
        std::cout << "[MAYAMESH]: (" << landmark.x << "," << landmark.y << "," << landmark.z << ")\n";

        std::string jointName = "landmark_joint_" + std::to_string(i);
        MObject joint = createJoint(jointPos, jointName);
        jointObjects.append(joint);
    }

    createSkinCluster(jointObjects, muscle);
    
    return MS::kSuccess;
}

MStatus MayaMesh::createSkinCluster(const MObjectArray& joints, const MObject& skinMesh)
{
    MStatus status;

    // Get joint names
    MString jointNamesCmd = "";
    for (unsigned int i = 0; i < joints.length(); ++i)
    {
        MFnDependencyNode jointFn(joints[i]);
        jointNamesCmd += jointFn.name() + " ";
    }

    // Get mesh name
    MFnDependencyNode meshFn(skinMesh);
    MString meshName = meshFn.name();

    // build the mel command
    MString cmd = "skinCluster -toSelectedBones ";
    cmd += jointNamesCmd + meshName;

    //execute the command
    status = MGlobal::executeCommand(cmd, true);
    if (status != MS::kSuccess) {
        MGlobal::displayError("Failed to create skinCluster via command: " + cmd);
        return status;
    }

    MGlobal::displayInfo("SkinCluster created.");
    return MS::kSuccess;
}

MStatus MayaMesh::muscleDeformation( const std::unordered_map<int,std::vector<ActionUnitDelta>>& auDeltaTable, const std::optional<landmarksDistanceData>& activeAU_opt)
{

    std::cout << "inside the muscle deformation";
    if (!activeAU_opt.has_value())
        return MS::kSuccess;

    int   auId      = activeAU_opt->auId;
    std::cout << "auId  : " << auId;
    float intensity = activeAU_opt->intensity;
    std::cout << "intensity : " << intensity;

    if (_muscleShape == MObject::kNullObj)
        return MS::kFailure;

    // 1) Build MFnMesh
    MStatus status;
    MDagPath dagPath;
    status = MDagPath::getAPathTo(_muscleShape, dagPath);
    if (status != MS::kSuccess) return status;
    status = dagPath.extendToShape();
    if (status != MS::kSuccess) return status;

    MFnMesh meshFn(dagPath, &status);
    if (status != MS::kSuccess) return status;

    std::cout <<  "read points part ";
    // 2) Read points
    MFloatPointArray points;
    status = meshFn.getPoints(points, MSpace::kWorld);
    if (status != MS::kSuccess) return status;

    // 3) Number of verts
    const unsigned vertCount = points.length();

    // 4) Prepare prevAccums buffer
    _prevAccums.setLength(vertCount);
    for (unsigned i = 0; i < vertCount; ++i) {
        _prevAccums[i] = MVector(0.0, 0.0, 0.0);
    }

    // 5) Build currAccums
    MVectorArray currAccums;
    currAccums.setLength(vertCount);
    for (unsigned i = 0; i < vertCount; ++i) {
        currAccums[i] = MVector(0.0, 0.0, 0.0);
    }

    // 6) Accumulate
    auto it = auDeltaTable.find(auId);
    if (it != auDeltaTable.end()) {
        const auto& auList = it->second;
        auto accumulateMuscles = [&](auto const& muscles) {
            for (auto const& m : muscles) {
                for (auto const& vd : m.deltas) {
                    int idx = vd.vertexIndex;
                    if (idx < 0 || idx >= (int)vertCount) continue;
                    currAccums[idx] += MVector(vd.delta.x,
                                               vd.delta.y,
                                               vd.delta.z);
                }
            }
        };
        for (auto const& au : auList) {
            accumulateMuscles(au.activeMuscles);
            accumulateMuscles(au.passiveMuscles);
        }
    }

    // 7) Apply delta-of-delta
    for (unsigned i = 0; i < vertCount; ++i) {
        MVector diff = currAccums[i] - _prevAccums[i];
        points[i].x += diff.x * intensity;
        points[i].y += diff.y * intensity;
        points[i].z += diff.z * intensity;
    }

    // 8) Write back (reuse existing 'status')
    status = meshFn.setPoints(points, MSpace::kWorld);
    if (status == MS::kSuccess) {
        _prevAccums = currAccums;
    }
    return status;
}