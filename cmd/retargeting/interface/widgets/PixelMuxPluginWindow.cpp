#include "PixelMuxPluginWindow.h"

#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MStringArray.h>

#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <filesystem>

PixelMuxWindow::PixelMuxWindow(const std::string& pluginDir, QWidget* parent)
    : QMainWindow(parent), m_pluginDir(pluginDir) {
    m_MayaMesh = std::make_unique<MayaMesh>(); // to initialize the pointer to MayaMesh class
    m_ActionUnit = std::make_unique<ActionUnit>(); // to initialize the pointer to ActionUnit class
    m_FacialLandmark = std::make_unique<FacialLandmark>(); // to initialize the pointer to Facial Landmarks
    m_DCCInterface = std::make_unique<DCCInterface>(m_ActionUnit.get(), m_FacialLandmark.get()); // to initialize the pointer to the DCC Class

    setWindowTitle("PixelMux Retargeting Plug-in");
    resize(450, 150);

    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    
    // Upload mesh 
    QHBoxLayout* uploadModelLayout = new QHBoxLayout();
    QPushButton* uploadModelBtn = new QPushButton("Upload Model");

    connect(uploadModelBtn, &QPushButton::clicked, this, &PixelMuxWindow::onUploadModel);

    // Upload buttons
    QHBoxLayout* uploadLayout = new QHBoxLayout();
    QPushButton* uploadPortraitBtn = new QPushButton("Upload Portrait");
    QPushButton* uploadAudioBtn = new QPushButton("Upload Audio");

    connect(uploadPortraitBtn, &QPushButton::clicked, this, &PixelMuxWindow::onUploadPortrait);
    connect(uploadAudioBtn, &QPushButton::clicked, this, &PixelMuxWindow::onUploadAudio);

    uploadLayout->addWidget(uploadPortraitBtn);
    uploadLayout->addWidget(uploadAudioBtn);
    uploadLayout->addWidget(uploadModelBtn);
    mainLayout->addLayout(uploadLayout);

    // Generate button 
    QPushButton* generateBtn = new QPushButton("Generate Animation Data");
    connect(generateBtn, &QPushButton::clicked, this, &PixelMuxWindow::onGenerate);
    mainLayout->addWidget(generateBtn);

    setCentralWidget(central);

// ---- Initialization proces ---- //
    std::string musclePathStr = m_pluginDir + "/retargeting/data/musclePatches.json";
    const char* musclePath = musclePathStr.c_str();

    std::string deltaPathStr = m_pluginDir + "/retargeting/data/deltaTransfer.json";
    const char* deltaPath = deltaPathStr.c_str();

    std::string landmarksMeshPathStr = m_pluginDir + "/retargeting/data/landmarksMeshIndex.json";
    const char* landmarksMeshPath = landmarksMeshPathStr.c_str();
    
    std::string landmarksPixelPathStr = m_pluginDir + "/retargeting/data/landmarksPixelIndex.json";
    const char* landmarksPixelPath = landmarksPixelPathStr.c_str();

    std::string landmarksActionUnitStr = m_pluginDir + "/retargeting/data/landmarksActionUnits.json";
    const char* landmarksActionUnit = landmarksActionUnitStr.c_str();

    uploadingMusclePatches(musclePath);
    uploadingDeltaTransfer(deltaPath); 
    uploadingLandmarksMeshIndex(landmarksMeshPath); // This is the landmark index data for the 3D mesh 
    uploadingLandmarksPixelIndex(landmarksPixelPath); // This is the landmark index data for pixel-mux system
    uploadinglandmarksActionUnitsMapping(landmarksActionUnit); // This is the data to mapping landmarks pair and action units
}

void PixelMuxWindow::onUploadPortrait() {
    m_portraitPath = QFileDialog::getOpenFileName(this, "Select portrait", "", "Imagen (*.jpg)");
    qDebug() << "[PIXELMUXWINDOW] portrait path is: " << m_portraitPath;
}

void PixelMuxWindow::onUploadAudio() {
    m_audioPath = QFileDialog::getOpenFileName(this, "Select audio", "", "Audio (*.wav)");
    qDebug() << "[PIXELMUXWINDOW] audio path is: " << m_audioPath;
}

void PixelMuxWindow::onUploadModel() {
    m_modelPath = QFileDialog::getOpenFileName(this, "Select model", "", "Model (*.obj)");
    qDebug()  << "[PIXELMUXWINDOW] model path is: "<< m_modelPath;
}

void PixelMuxWindow::onGenerate() {
    if (m_portraitPath.isEmpty() || m_audioPath.isEmpty() || m_modelPath.isEmpty() ) {
        QMessageBox::warning(this, "Input files missing", "Please upload the model, portrait and the audio before press generate.");
        return;
    }

   // ---- Main processing steps: model loading, mesh preparation, landmark extraction, landmark evaluation, animation driver---- //

    // Convert the model path to MString and load it in the Maya viewport (muscle template reference)
    m_modelPathMString = m_MayaMesh->convertModelPathToMString(m_modelPath);    
    m_MayaMesh->loadMayaMuscle(m_modelPathMString);

    // Convert the model path to a standard C++ string
    m_modelPathString = m_DCCInterface->convertModelPathToString(m_modelPath);

    // Load input mesh vertices into m_meshInputVertices
    m_DCCInterface->processInputMesh(m_modelPathString);

    // Map muscle IDs to their corresponding vertices in the input mesh
    m_DCCInterface->getMeshMuscles();

    // Extract 3D landmark vertices (51 total) from the input mesh (stored in m_inputMeshLandmarks3D)
    m_DCCInterface->getInputMeshLandmarks3D();

    // Get 478-point landmarks from the neutral frame (stored in m_generatedNeutralLandmarks)

    std::string NeutralFaceDataJsonStr = m_pluginDir + "/retargeting/landmarks-data/NeutralFace.json"; //this can be fixed just moving the folder to the build plugin one as data!!! 
    const char* NeutralFaceDataJson = NeutralFaceDataJsonStr.c_str();
    m_DCCInterface->processNeutralFaceData(NeutralFaceDataJson);

    // 51-point subset landmarks from the neutral frame (stored in m_neutralLandmarks51)
    m_DCCInterface->get51SetLandmarksNeutralFace();
    
    // Get the landmarks to action unit map
    const auto& landmarksAuMap = m_FacialLandmark->getLandmarksActionUnitMap(); // map that contains the relation between landmarks and action units

    // Get the euclidian distance between landmarks in neutral face (I need to check the ones with tree entries)
    m_DCCInterface->computeLandmarksNeutralDistanceData(landmarksAuMap);

    // Get 478-point landmarks from the pose/current frame (stored in m_generatedCurrentLandmarks)
    std::string CurrentFaceDataJsonStr = m_pluginDir + "/retargeting/landmarks-data/Pose1.json";
    const char* CurrentFaceDataJson = CurrentFaceDataJsonStr.c_str();
    auto currentFaceData = m_DCCInterface->processCurrentFaceData(CurrentFaceDataJson);

    // 51-point subset landmarks from the current frame (stored in m_currentFaceVertices)
    m_DCCInterface->get51SetLandmarksCurrentFace();
    
    // Evaluate what is the distance between pair of landmarks 
    m_DCCInterface->computeLandmarksCurrentDistanceData(landmarksAuMap);

    float minThreshold = 0.4f; // minimun value to detect activation (TODO: should be identified based on data and dynamically)
    float maxThreshold = 0.6f; // maximum value of activation (TODO: should be identified based on data and dynamically)

    // Evaluate the distance of the current face and the neutral face to identify what action units is being activate it
    auto currentAUActivate = m_DCCInterface->evaluateActivatedAUs(minThreshold, maxThreshold);
    std::cout << "[PIXELMUXWINDOW] the current AU acticate is:  " << "\n";
    std::cout << "AU Id: "     << currentAUActivate->auId  << "  Intensity: " << currentAUActivate->intensity << "\n";

    auto inputMeshLandmarks = m_DCCInterface->returnInputMeshLandmarks3D();
    std::cout << "[PIXELMUXWINDOW] The size of the input Mesh Landmarks 3D is: " << inputMeshLandmarks.size() << "landmarks entries." << "\n";

    // Do the skinning process and joint based on the landmarks capture in the input mesh 
    m_MayaMesh->prepareMeshSkinning(inputMeshLandmarks);
    
    //-------- Animation driving approach -------//
    // get the map relation between au and delta transfer
    auto auDeltaTable = m_ActionUnit->getAuDeltaTable(); 
    
    // Deform the muscle mesh based on intensity and delta transfer of the active and passive muscles of the current Active Action Unit)    
    std::cout << "[PIXELMUXWINDOW] muscle deformation before"; 
    m_MayaMesh->muscleDeformation(auDeltaTable, currentAUActivate);

    //TODO: Import the skin mesh
    //TODO: Apply proxmity transfer from muscle rig to skin mesh

    showProcessingDialog();
    simulateAPICall();
}

void PixelMuxWindow::showProcessingDialog() {
    QMessageBox* processing = new QMessageBox(QMessageBox::Information,
                                               "Processing",
                                               "Generating animation data from PixelAPI...",
                                               QMessageBox::NoButton,
                                               this);
    processing->show();
    QTimer::singleShot(2000, processing, &QMessageBox::accept); // simulation window
}

void PixelMuxWindow::simulateAPICall() {
    // Call and update Maya
    MGlobal::displayInfo("Data ready!");
}

void PixelMuxWindow::uploadingMusclePatches(const char* musclesJson) {
    m_ActionUnit->loadMuscleIndexMapFromJSON(musclesJson);
}

void PixelMuxWindow::uploadingModelsPath(const char* modelsJson, const char* basePath) {
    // this should be run one time in the pre-processing to generate the data we are going to use in the deltaTransfer
    m_ActionUnit->loadModelPathsFromJSON(modelsJson, basePath);
}

void PixelMuxWindow::uploadingDeltaTransfer(const char* deltaTransferJson) {
    m_ActionUnit->loadDeltaTransfersFromJSON(deltaTransferJson);
}

void PixelMuxWindow::uploadingLandmarksMeshIndex(const char* landmarksMeshJson) {
    m_FacialLandmark->loadLandmarksMeshIndexFromJSON(landmarksMeshJson);
}

void PixelMuxWindow::uploadingLandmarksPixelIndex(const char* landmarksPixelJson) {
    m_FacialLandmark->loadLandmarksPixelIndexFromJSON(landmarksPixelJson);
}

void PixelMuxWindow::uploadinglandmarksActionUnitsMapping(const char* landmarksAUJson)
{
    m_FacialLandmark->loadLandmarksActionUnitsMappingFromJson(landmarksAUJson);
}