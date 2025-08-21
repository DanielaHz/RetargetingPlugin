#ifndef DCCINTERFACE_H_
#define DCCINTERFACE_H_

#include <filesystem>
#include <memory>
#include <QString>
#include <QDebug>
#include "FacialMesh.h"
#include "ActionUnit.h"
#include "FacialLandmark.h"
#include "MathUtils.h"
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <unordered_map>
#include <vector>
#include "Side.h"
#include <optional>

// This class connects the data received from PixelMuxPluginWindow
// with the animation data generation API and the backend.
// It coordinates the internal logic and controls the plugin.

/**
 * @class DCCInterface
 * @brief Core logic controller for the PixelMux plugin.
 *
 * This class acts as the bridge between the UI (PixelMuxWindow) and the animation backend.
 * It handles mesh processing, landmark extraction, and muscle mapping, and prepares data for animation generation.
 */
struct landmarksDistanceData 
{
  int auId;
  std::string side;
  std::vector<int> landmarkIndices;
  float baseDistance = 0.0f;
  float currentDistance = 0.0f;
  float activationThreshold = 1.0f;
  bool isActive = false;
  float intensity = 0.0f;
};
class DCCInterface 
{
    public:
    /**
    * @brief Constructs the DCCInterface with references to core animation components.
    * @param actionUnit Pointer to the ActionUnit instance for managing facial expressions.
    * @param facialLandmark Pointer to the FacialLandmark instance for landmark detection and mapping.
    */
    DCCInterface(ActionUnit* actionUnit, FacialLandmark* facialLandmark)
        : m_actionUnit(actionUnit), m_facialLandmark(facialLandmark) {}

    /**
     * @brief Converts a Qt QString path to a standard std::string.
     * @param path The input file path from the UI.
     * @return Converted std::string path.
     */
    std::string convertModelPathToString(QString &path); // Converts the QString path recieved from the UI to std::string
    
    /**
    * @brief Processes the input mesh model from the given path.
    * @param path Path to the 3D model file.
    */
    void processInputMesh(const std::string &path); // Call the functions to process the input Model
    
    /**
    * @brief Prints mesh vertex data for debugging or inspection.
    * @param vector Vector of mesh vertices.
    * @param num Number of vertices to print.
    */
    void printMeshVertices(std::vector<glm::vec3> &vector, size_t &num);

    /**
    * @brief Extracts muscle vertex data from the mesh.
    */
    void getMeshMuscles(); 
    
    /**
     * @brief Prints the muscle vertex map for debugging.
     * @param map Mapping of muscle indices to vertex groups.
     */
    void printMapMuscles(std::unordered_map<int, std::vector<glm::vec3>>& map);
    
    /**
     * @brief Extracts 51 landmark vertices from the mesh.
     */
     
    void getInputMeshLandmarks3D();

    /**
     * @brief Processes landmark data received from the backend.
     * @param landmarksDataJson JSON string containing landmark data.
     * @return True if processing was successful, false otherwise.
     */
    bool processNeutralFaceData(const char* landmarksDataJson);


    bool processCurrentFaceData(const char* landmarksDataJson);
    /**
     * @brief Extracts pixel-based landmark vertices from input data.
     */
    void get51SetLandmarksNeutralFace();

    void get51SetLandmarksCurrentFace();

    /**
     * @brief Compute distances using the internally stored AU‐map.
     */
    void computeLandmarksNeutralDistanceData(const std::unordered_map<int,std::vector<landmarksActionUnit>>& landmarksAUMap);
    void computeLandmarksCurrentDistanceData(const std::unordered_map<int,std::vector<landmarksActionUnit>>& landmarksAUMap);
    
    
    std::optional<landmarksDistanceData> evaluateActivatedAUs(float thresholdMin, float thresholdMax);

    float calculateIntensity(float thresholdMin, float thresholdMax, float currentDistance, float baseDistance);

    std::vector<glm::vec3> returnInputMeshLandmarks3D(); // to be used in the skinning part in MayaMesh

    std::unordered_map<int, std::vector<glm::vec3>> returnMapMuscleVertices();
    private:

    // Mesh and landmark data containers
    std::vector<glm::vec3> m_meshInputVertices;                             ///< Store the input mesh vertices recieved from the UI (5898 vertices)
    std::unordered_map<int, std::vector<glm::vec3>> m_mapMuscleVertices;    ///< Muscle vertex mapping
    std::vector<glm::vec3> m_inputMeshLandmarks3D;                          ///< 3D landmarks extracted from the input mesh

    std::vector<glm::vec3> m_generatedNeutralLandmarks;    ///< 3D landmarks from the neutral frame of the generated video (478 Landmarks)
    std::vector<glm::vec3> m_generatedCurrentLandmarks;      ///< 3D landmarks from a specific pose/frame in the generated video

    std::unordered_map<int, std::vector<glm::vec3>> m_mapLandmarksActionUnitVertices;    ///< landmarks to action unit vertex ma    std::unordered_map<int, std::vector<glm::vec3>> returnMapLandmarksActionUnitVertices(); 

    std::vector<landmarksDistanceData> m_landmarksDistanceNeutraResults;
    std::vector<landmarksDistanceData> m_landmarksDistanceCurrentResults;

    std::vector<glm::vec3> m_neutralFaceVertices;        ///< Subset of 51 pixel landmarks used for animation
    std::vector<glm::vec3> m_currentFaceVertices;        ///< Subset of 51 pixel landmarks used for animation
   
    FacialMesh m_facialMesh;                                ///< Facial mesh processor
    ActionUnit* m_actionUnit;                               ///< Pointer to facial action unit manager
    FacialLandmark* m_facialLandmark;                       ///< Pointer to facial landmark manager
    MathUtils m_mathUtils;
    QString m_modelPath;                                    ///< Path to the input model (Qt format)
};

#endif
