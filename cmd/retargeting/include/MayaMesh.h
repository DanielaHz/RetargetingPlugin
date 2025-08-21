#ifndef MAYAMESH_H_
#define MAYAMESH_H

#include <maya/MGlobal.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MObject.h>
#include <maya/MFnDagNode.h>
#include <maya/MStatus.h>
#include <QString>
#include <maya/MString.h>
#include <maya/MFnSkinCluster.h>
#include <glm/vec3.hpp>
#include <vector>
#include <maya/MObjectArray.h>
#include <maya/MFnIkJoint.h>
#include <maya/MVector.h>
#include <maya/MItGeometry.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MDoubleArray.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <optional>
#include <unordered_map>
#include <DCCInterface.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MTypes.h> 
#include <maya/MFloatPointArray.h>   
#include <maya/MFloatVector.h>

/**
 * @class MayaMesh
 * @brief Handles mesh operations within the Maya viewport for the PixelMux plugin.
 *
 * This class is responsible for importing OBJ files, renaming and positioning mesh objects,
 * and placing anatomical reference meshes such as the skull. It supports automatic alignment
 * of skin and muscle meshes to facilitate proximity wrap deformation workflows.
 */
class MayaMesh {
public:
    /**
     * @brief Converts a Qt QString path to Maya's MString format.
     * @param path The input file path from the UI.
     * @return Converted MString path.
     */
    MString convertModelPathToMString(const QString& path);

    /**
     * @brief Renames a mesh transform node in the Maya scene.
     * @param transform The transform node to rename.
     * @param newName The new name to assign.
     * @return Maya status indicating success or failure.
     */
    MStatus renameMesh(const MObject& transform, const MString& newName);

    /**
     * @brief Positions a transform node in the Maya scene.
     * Used to align skin, muscle, and skull meshes.
     * @param transform The transform node to move.
     * @param position The target position vector.
     * @param space The coordinate space (e.g., world or object).
     * @return Maya status indicating success or failure.
     */
    MStatus positionTransform(const MObject& transform, const MVector& position, MSpace::Space space);

    /**
     * @brief Loads a mesh from an OBJ file into the Maya scene.
     * @param objPath Path to the OBJ file.
     * @return Maya status indicating success or failure.
     */

    MStatus loadMayaMuscle(const MString& objPath);
    MStatus loadMayaSkin(const MString& objPath);
    MStatus loadMayaSkull(const MString& objPath); // this should be scale based on the size of the meshs

    MObject getMayaMuscle();

    MObject createJoint(const MVector& position, const std::string& name);
    MStatus createSkinCluster(const MObjectArray& joints, const MObject& skinMesh);
    MStatus prepareMeshSkinning(std::vector<glm::vec3> m_inputMeshLandmarks3D);
    MStatus muscleDeformation(const std::unordered_map<int, std::vector<ActionUnitDelta>>& auDeltaTable, const std::optional<landmarksDistanceData>& activeAU_opt);

    /**
     * @brief Imports an OBJ mesh and retrieves its transform and shape nodes.
     * @param objPath Path to the OBJ file.
     * @param transformOut Output reference to the transform node.
     * @param shapeOut Output reference to the shape node.
     * @return Maya status indicating success or failure.
     */
    MStatus importObjMesh(const MString& objPath, MObject& transformOut, MObject& shapeOut, std::string& nameOut);

    /**
     * @brief Places a default skull mesh beneath the muscle mesh.
     * This helps constrain deformation and maintain anatomical accuracy.
     * @return Maya status indicating success or failure.
     */

    MObject findMeshByName(const MString& meshName, MStatus* outStatus) const
    {
        MSelectionList sel;
        MStatus        s = sel.add(meshName);
        if (outStatus) *outStatus = s;
        if (s != MS::kSuccess) return MObject::kNullObj;

        MObject node;
        sel.getDependNode(0, node);
        return node;
    }

private:
    MObject _skinTransform{ MObject::kNullObj };
    MObject _skinShape{ MObject::kNullObj };

    MObject _muscleTransform{ MObject::kNullObj };
    MObject _muscleShape{ MObject::kNullObj };

    MObject _skullTransform{ MObject::kNullObj };
    MObject _skullShape{ MObject::kNullObj };

    MVectorArray _prevAccums;   // stores last frame's accumulations
};

#endif