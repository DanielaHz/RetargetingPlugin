#include "FacialLandmark.h"

bool FacialLandmark::loadLandmarksMeshIndexFromJSON(const char* landmarksMeshJson)
{
    m_landmarksMeshIndex.clear();

    // read the json files
    std::string fileLandmarkMeshStr = std::string(landmarksMeshJson);
    std::ifstream file(fileLandmarkMeshStr);
    if (!file.is_open()){
        std::cerr << "Failed to open file landmarks 3D Model: " << fileLandmarkMeshStr << "\n";
        return false;
    }

    nlohmann::json dataModel;

    try {
        file >> dataModel;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return false;
    }

    // save the file in the vector 
    for (const auto& index : dataModel) {
        m_landmarksMeshIndex.push_back(index);
    }
    std::cout << "[Loader][FACIALLANDMARK]: reading file: landmarksMeshIndex.json. The data was stored in a vector with size of : " << m_landmarksMeshIndex.size() << " landmarks entries. "<< "\n";
    return true;
}

bool FacialLandmark::loadLandmarksPixelIndexFromJSON(const char* landmarksPixelJson)
{
    m_landmarksPixelIndex.clear();

    // read the json files
    std::string fileLandmarkPixelStr = std::string(landmarksPixelJson);
    std::ifstream file(fileLandmarkPixelStr);
    if (!file.is_open()){
        std::cerr << "Failed to open file landmarks Pixel: " << fileLandmarkPixelStr << "\n";
        return false;
    }

    nlohmann::json dataModel;

    try {
        file >> dataModel;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return false;
    }

    // save the file in the vector 
    for (const auto& index : dataModel) {
        m_landmarksPixelIndex.push_back(index);
    }
    std::cout << "[Loader][FACIALLANDMARK]: reading file: landmarksPixelIndex.json. The data was stored in a vector of size : " << m_landmarksPixelIndex.size() << " landmarks entries. "<< "\n";
    return true;
}

std::vector<int> FacialLandmark::getLandmarksMeshIndex()
{
    return m_landmarksMeshIndex;
}

std::vector<int> FacialLandmark::getLandmarksPixelIndex()
{
    return m_landmarksPixelIndex;
}

bool FacialLandmark::loadLandmarksActionUnitsMappingFromJson(const char* path) {
    m_landmarksActionUnitMap.clear();

    std::cout << "[Loader][FACIALLANDMARK] Loading file: landmarksActionUnits.json from path: " << path << "\n";

    // Verifica existencia y abre archivo
    if (!std::filesystem::exists(path)) {
        std::cerr << "[Loader] ERROR: file: landmarksActionUnits.json does not exist\n";
        return false;
    }

    std::ifstream ifs(path);
    if (!ifs) {
        std::cerr << "[Loader] ERROR: failed to open file: landmarksActionUnits.json\n";
        return false;
    }

    // Parse JSON
    nlohmann::json root;
    try {
        ifs >> root;
    } catch (const std::exception& e) {
        std::cerr << "[Loader][FACIALLANDMARK] JSON parse error: " << e.what() << "\n";
        return false;
    }

    // Validación mínima de estructura
    if (!root.contains("mappings") || !root["mappings"].is_array()) {
        std::cerr << "[Loader] ERROR: missing or invalid \"mappings\" array\n";
        return false;
    }

    int entryIdx = 0;
    for (const auto& elem : root["mappings"]) {
        ++entryIdx;
        try {
            int auId = elem.at("auId").get<int>();
            std::string sideStr = elem.at("side").get<std::string>();
            std::vector<int> indices = elem.at("landmarkIndices").get<std::vector<int>>();

            Side side = sideFromString(sideStr); // O FacialLandmark::sideFromString(...)
            landmarksActionUnit landmarkAU{auId, side, std::move(indices)};

            m_landmarksActionUnitMap[auId].push_back(std::move(landmarkAU));
        }
        catch (const std::exception& e) {
            std::cerr << "[Loader] ERROR in entry #" << entryIdx << ": " << e.what() << "\n";
        }
    }
    for (const auto& [auId, landmarkUnits] : m_landmarksActionUnitMap)
    {
        for (const auto& unit : landmarkUnits)
        {
            std::cout << "[Loader][FACIALLANDMARK]: Checking AU " << auId << ", side: " << sideToString(unit.side) << ", landmarks: " << unit.landmarkIndices.size() << "\n";
        }
    }

    std::cout << "[Loader][FACIALLANDMARKS] The total of AUs loaded are : " << m_landmarksActionUnitMap.size() << "\n";
    return true;
}

const std::unordered_map <int, std::vector<landmarksActionUnit>> FacialLandmark::getLandmarksActionUnitMap()
{
    return m_landmarksActionUnitMap;
}