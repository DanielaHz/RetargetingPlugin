#include <iostream>
#include "DCCInterface.h"

std::string DCCInterface::convertModelPathToString(QString &path)
{
    QString cleanedPath = path;
    if (cleanedPath.startsWith("\"") && cleanedPath.endsWith("\"")) {
        cleanedPath = cleanedPath.mid(1, cleanedPath.length() - 2);
    }

    std::string modelPath = cleanedPath.toStdString();  
    return modelPath;
}

void DCCInterface::processInputMesh(const std::string &path)
{
    std::cout << "[DCCInterface] Validating mesh input file path: " << path << std::endl;

    if (!std::filesystem::exists(path)) {
        std::cerr << "[ERROR] File does not exist: " << path << "\n";
        return;
    }

    if (!std::filesystem::is_regular_file(path)) {
        std::cerr << "[ERROR] File is not a regular file: " << path << "\n";
        return;
    }

    // Load the mesh vertices from the given file path
    m_meshInputVertices = m_facialMesh.loadModel(path.c_str());

    // Print the first 5 vertices for verification
    size_t numToPrint = 5;
    printMeshVertices(m_meshInputVertices, numToPrint);
}


void DCCInterface::printMeshVertices(std::vector<glm::vec3> &vector, size_t &num)
{
    std::cout << "[DCCInterface] printing the first : " << num << " vertices in the input mesh" << "\n";
    for (size_t t = 0; t < num ; ++t)
    {
        std::cout << "vertex: " << t << "(" << vector[t].x << "," << vector[t].y << "," << vector[t].z << ")" << "\n";
    }
}

void DCCInterface::getMeshMuscles()
{
    // Clear the existing map to avoid using outdated or corrupted data
    m_mapMuscleVertices.clear();

    // Retrieve the mapping from muscle IDs to vertex indices
    std::unordered_map<int, std::vector<int>> musclesMap = m_actionUnit->getMuscleIndexMap();

    if (musclesMap.empty()) {
        std::cout << "[DCCInterface] Muscle index map from ActionUnit is empty.\n";
    }

    for (const auto& [muscleKey, vertexIndices] : musclesMap)
    {
        std::vector<glm::vec3> muscleVertices;

        for (int index : vertexIndices)
        {
            if (index >= 0 && index < static_cast<int>(m_meshInputVertices.size()))
            {
                muscleVertices.push_back(m_meshInputVertices[index]);
            }
            else
            {
                std::cerr << "[ERROR] Muscle vertex index out of range: " << index << "\n";
            }
        }

        m_mapMuscleVertices[muscleKey] = std::move(muscleVertices);
        std::cout << "[DCCInterface] Muscle ID " << muscleKey 
                  << " has " << m_mapMuscleVertices[muscleKey].size() 
                  << " associated vertices.\n";
    }
}


void DCCInterface::printMapMuscles(std::unordered_map<int, std::vector<glm::vec3>>& map)
{
    for (auto& value: map)
    {
        std::cout << "The muscles number: "<< value.first << " have this vertices: " << "\n";
        for (auto second: value.second) { std::cout << second.x << "," << second.y << "," << second.z << ")" << "\n";}
    }
}

void DCCInterface::getInputMeshLandmarks3D()
{
    m_inputMeshLandmarks3D.clear();

    // get mesh landmarks indices 
    std::vector<int> landmarksIndex = m_facialLandmark->getLandmarksMeshIndex();
    if(landmarksIndex.empty())
    {
        std::cout << "[DCCInterface][ERROR]: The landmarks index vector is empty" << "\n";
    }

    for(auto index: landmarksIndex)
    {
        glm::vec3 verticesxindex = m_meshInputVertices[index];
        m_inputMeshLandmarks3D.push_back(verticesxindex);
    }
    std::cout << "[DCCInterface] The input mesh landmarks 3D size is: " << m_inputMeshLandmarks3D.size() << " entries. "<< "\n";
}

bool DCCInterface::processNeutralFaceData(const char* landmarksDataJson)
{
    m_generatedNeutralLandmarks.clear();

    std::string filePathStr = std::string(landmarksDataJson);
    std::ifstream file(filePathStr);
    if (!file.is_open()) {
        std::cerr << "[DCCInterface][DEBUG] Failed to open file: " << filePathStr << "\n";
        return false;
    }

    nlohmann::json data;
    try {
        file >> data;
    } catch (const std::exception& e) {
        std::cerr << "[DCCInterface] JSON landmarks data parse error: " << e.what() << "\n";
        return false;
    }

    for(const auto& group: data["data"])
    {
        for (const auto& vertex: group)
        {
            glm::vec3 point;
            point.x = vertex["x"];
            point.y = vertex["y"];
            point.z = vertex["z"];

            m_generatedNeutralLandmarks.push_back(point);
        }
    }
    std::cout << "[DCCInterface] The size of the neutral face landmarks is: " << m_generatedNeutralLandmarks.size() << " entries. "<< "\n";
    return true;
}

void DCCInterface::get51SetLandmarksNeutralFace()
{
    // get pixel landmarks indices 
    std::vector<int> landmarksIndex = m_facialLandmark->getLandmarksPixelIndex();
    if(landmarksIndex.empty())
    {
        std::cout << "[DCCInterface][ERROR]: The vector landmarks pixel index is empty" << "\n";
    }

    if(m_generatedNeutralLandmarks.empty())
    {
        std::cout << "[DCCInterface][ERROR]: The generated neutral landmarks vector is empty" << "\n";
    }

    // extract the data of the 51 according with the pixel index 
    for(auto index: landmarksIndex)
    {
        glm::vec3 verticesxindex = m_generatedNeutralLandmarks[index];
        m_neutralFaceVertices.push_back(verticesxindex);
    }
    std::cout << "[DCCInterface]: The subset vector neutral face have : " << m_neutralFaceVertices.size() << "landmarks entries. "<< "\n";
}


bool DCCInterface::processCurrentFaceData(const char* landmarksDataJson)
{
    m_generatedCurrentLandmarks.clear();

    std::string filePathStr = std::string(landmarksDataJson);
    std::ifstream file(filePathStr);
    if (!file.is_open()) {
        std::cerr << "[DCCInterface][DEBUG] Failed to open file: " << filePathStr << "\n";
        return false;
    }

    nlohmann::json data;
    try {
        file >> data;
    } catch (const std::exception& e) {
        std::cerr << "[DCCInterface] JSON landmarks data parse error: " << e.what() << "\n";
        return false;
    }

    for(const auto& group: data["data"])
    {
        for (const auto& vertex: group)
        {
            glm::vec3 point;
            point.x = vertex["x"];
            point.y = vertex["y"];
            point.z = vertex["z"];

            m_generatedCurrentLandmarks.push_back(point);
        }
    }
    std::cout << "[DCCInterface] The size of the current face landmarks vector is: " << m_generatedCurrentLandmarks.size() << " entries. "<< "\n";
    return true;
}

void DCCInterface::get51SetLandmarksCurrentFace()
{
    // get pixel landmarks indices 
    std::vector<int> landmarksIndex = m_facialLandmark->getLandmarksPixelIndex();
    if(landmarksIndex.empty())
    {
        std::cout << "[DCCInterface][ERROR]: The vector landmarks pixel index is empty" << "\n";
    }

    if(m_generatedCurrentLandmarks.empty())
    {
        std::cout << "[DCCInterface][ERROR]: The generated neutral landmarks vector is empty" << "\n";
    }

    // extract the data of the 51 according with the pixel index 
    for(auto index: landmarksIndex)
    {
        glm::vec3 verticesxindex = m_generatedCurrentLandmarks[index];
        m_currentFaceVertices.push_back(verticesxindex);
    }
    std::cout << "[DCCInterface]: The subset vector for current face have : " << m_currentFaceVertices.size() << " landmarks entries. "<< "\n";
}

void DCCInterface::computeLandmarksNeutralDistanceData(const std::unordered_map<int,std::vector<landmarksActionUnit>>& landmarksAUMap)
{
    for (const auto& [auId, landmarkUnits] : landmarksAUMap)
    {
        for (const auto& unit : landmarkUnits)
        {
            std::cout << "[DCCInterface]: [DEBUG] Checking AU " << auId << ", side: " << sideToString(unit.side) << ", landmarks: " << unit.landmarkIndices.size() << "\n";
        }
    }

    for (const auto& [auId, auGroup] : landmarksAUMap)
    {
        for (const auto& landmarksAU : auGroup)
        {
            const auto& indices = landmarksAU.landmarkIndices;
            if (indices.size() < 2) continue;

            landmarksDistanceData result;
            result.auId = landmarksAU.auId;
            result.side = sideToString(landmarksAU.side);
            result.landmarkIndices = indices;

            if (indices.size() % 2 == 0)
            {
                for (size_t i = 0; i < indices.size() - 1; i += 2)
                {
                    size_t indexA = indices[i];
                    size_t indexB = indices[i + 1];

                    if (indexA >= m_neutralFaceVertices.size() || indexB >= m_neutralFaceVertices.size()) {
                        std::cout << "[WARNING] Invalid landmark index: " << indexA << " or " << indexB << "\n";
                        continue;
                    }

                    glm::vec3 vertexA = m_neutralFaceVertices[indexA];
                    glm::vec3 vertexB = m_neutralFaceVertices[indexB];

                    float distance = m_mathUtils.calculateEuclidianDistance(vertexA, vertexB);
         
                    std::cout << "[DCCInterface]: AU " << result.auId << " - Distance between " << indexA << " and " << indexB << ": " << distance << "\n";
                }
            }
            else
            {
                std::cout << "[DCCInterface]: [INFO] Landmark group has odd number of points: AU " << result.auId << ", side: " << result.side << "\n";
            }

            m_landmarksDistanceNeutraResults.push_back(result);
        }
    }
}

void DCCInterface::computeLandmarksCurrentDistanceData(const std::unordered_map<int,std::vector<landmarksActionUnit>>& landmarksAUMap)
{
    for (const auto& [auId, landmarkUnits] : landmarksAUMap)
    {
        for (const auto& unit : landmarkUnits)
        {
            std::cout << "[DCCInterface]: [DEBUG] Checking AU " << auId << ", side: " << sideToString(unit.side) << ", landmarks: " << unit.landmarkIndices.size() << "\n";
        }
    }

    for (const auto& [auId, auGroup] : landmarksAUMap)
    {
        for (const auto& landmarksAU : auGroup)
        {
            const auto& indices = landmarksAU.landmarkIndices;
            if (indices.size() < 2) continue;

            landmarksDistanceData result;
            result.auId = landmarksAU.auId;
            result.side = sideToString(landmarksAU.side);
            result.landmarkIndices = indices;

            if (indices.size() % 2 == 0)
            {
                for (size_t i = 0; i < indices.size() - 1; i += 2)
                {
                    size_t indexA = indices[i];
                    size_t indexB = indices[i + 1];

                    if (indexA >= m_generatedCurrentLandmarks.size() || indexB >= m_generatedCurrentLandmarks.size()) {
                        std::cout << "[WARNING] Invalid landmark index: " << indexA << " or " << indexB << "\n";
                        continue;
                    }

                    glm::vec3 vertexA = m_generatedCurrentLandmarks[indexA];
                    glm::vec3 vertexB = m_generatedCurrentLandmarks[indexB];

                    float distance = m_mathUtils.calculateEuclidianDistance(vertexA, vertexB);
                    result.currentDistance = distance;
         
                    std::cout << "[DCCInterface]: AU " << result.auId << " - Distance between " << indexA << " and " << indexB << ": " << distance << "\n";
                }
            }
            else
            {
                std::cout << "[DCCInterface]: [INFO] Landmark group has odd number of points: AU " << result.auId << ", side: " << result.side << "\n";
            }

            m_landmarksDistanceCurrentResults.push_back(result);
        }
    }
}

std::optional<landmarksDistanceData> DCCInterface::evaluateActivatedAUs(float thresholdMin, float thresholdMax)
{
    float maxDelta = -std::numeric_limits<float>::infinity(); // to store max
    size_t maxIndex = 0;

    if (m_landmarksDistanceNeutraResults.size() == m_landmarksDistanceCurrentResults.size())
    {
        for (size_t index = 0; index < m_landmarksDistanceNeutraResults.size(); index++)
        {
            float neutralDistance = m_landmarksDistanceNeutraResults[index].baseDistance;
            float currentDistance = m_landmarksDistanceCurrentResults[index].currentDistance;
            float delta = currentDistance - neutralDistance;

            if (delta > thresholdMin) // identifying the minimun activation 
            {
                std::cout << "The action unit at index " << index 
                          << " (AU id " << m_landmarksDistanceCurrentResults[index].auId << ") "
                          << " has a delta of " << delta
                          << " which exceeds the threshold minimum of: " << thresholdMin << "\n";

                if (delta > maxDelta) // just testing this with one action unit trigger
                {
                    maxDelta = delta;
                    maxIndex = index;
                }
            }
        }

        if (maxDelta > thresholdMin)
        {
            auto& resultMaxAU = m_landmarksDistanceCurrentResults[maxIndex];
            float neutralDistance = m_landmarksDistanceNeutraResults[maxIndex].baseDistance;
            float currentDistance = m_landmarksDistanceCurrentResults[maxIndex].currentDistance;
            
            float intensity = calculateIntensity(thresholdMin, thresholdMax, currentDistance, neutralDistance);

            resultMaxAU.intensity = intensity;
            resultMaxAU.isActive = true;

            std::cout << "\n==> Max activated AU:\n" 
            << "Index: " << maxIndex << "\n" 
            << "AU id: " << resultMaxAU.auId << "\n" 
            << "isActive: " << resultMaxAU.isActive << "\n" 
            << "AU intensity: " << resultMaxAU.intensity << "\n";
            return resultMaxAU;
        }
    }
    return std::nullopt;
}

float DCCInterface::calculateIntensity(float thresholdMin, float thresholdMax, float currentDistance, float baseDistance)
{
   return m_mathUtils.calculateIntensity(thresholdMin, thresholdMax, currentDistance, baseDistance);
}

 std::vector<glm::vec3> DCCInterface::returnInputMeshLandmarks3D()
 {
    return m_inputMeshLandmarks3D;
 }

 std::unordered_map<int, std::vector<glm::vec3>> DCCInterface::returnMapMuscleVertices()
{
    return m_mapLandmarksActionUnitVertices;
}