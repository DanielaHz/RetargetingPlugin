#ifndef PIXELMUXPLUGINWINDOW_H_
#define PIXELMUXPLUGINWINDOW_H_

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "DCCInterface.h"
#include "FacialLandmark.h"
#include "MayaMesh.h"
#include "ActionUnit.h"
#include <memory>
#include <filesystem>
#include <Side.h>

/**
 * @class PixelMuxWindow
 * @brief Main window class for the PixelMux plugin UI.
 *
 * This class provides a graphical interface for uploading a 3D model, portrait image, and audio file.
 * It also handles the generation of facial animation data by interfacing with Maya and other internal components.
 */

class PixelMuxWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructs the PixelMuxWindow.
     * @param pluginDir Path to the plugin directory used for internal resource resolution.
     * @param parent Optional parent widget.
     */
    explicit PixelMuxWindow(const std::string& pluginDir, QWidget* parent = nullptr);

private slots:
    /**
    * @brief Triggered when the user uploads a 3D model.
    */
    void onUploadModel();

    /**
     * @brief Triggered when the user uploads a portrait image.
     */
    void onUploadPortrait();

    /**
     * @brief Triggered when the user uploads an audio file.
     */
    void onUploadAudio();

    /**
     * @brief Triggered when the user clicks the "Generate" button.
     * Initiates the facial animation generation process.
     */
    void onGenerate();

private:
    // File paths selected by the user
    QString m_portraitPath;     ///< Path to the uploaded portrait image
    QString m_audioPath;        ///< Path to the uploaded audio file
    QString m_modelPath;        ///< Path to the uploaded 3D model

    // Internal representations of the model path
    std::string m_modelPathString; ///< Used by pkg/retargeting/FacialMesh for internal processing
    MString m_modelPathMString;    ///< Used by cmd/retargeting/MayaMesh to update Maya viewport

    std::string m_pluginDir; ///< Directory where the plugin is compiled realtive to build folder

    // Core processing components
    std::unique_ptr<DCCInterface> m_DCCInterface;       ///< Interface to the Digital Content Creation environment
    std::unique_ptr<MayaMesh> m_MayaMesh;               ///< Handles mesh operations in Maya
    std::unique_ptr<ActionUnit>m_ActionUnit;            ///< Manages facial action units for animation
    std::unique_ptr<FacialLandmark>m_FacialLandmark;    ///< Handles facial landmark detection data

    // Internal helper methods
    void showProcessingDialog(); ///< Displays a modal dialog indicating processing is underway
    void simulateAPICall();     ///< Simulates an API call (placeholder for actual backend integration)
      
    // Uploading helper methods for various JSON configurations
    void uploadingModelsPath(const char* modelsJson, const char* basePath);
    void uploadingMusclePatches(const char* musclesJson);
    void uploadingDeltaTransfer(const char* deltaTransferJson); 
    void uploadingLandmarksMeshIndex(const char* landmarksMeshJson);
    void uploadingLandmarksPixelIndex(const char* landmarksPixelJson);
    void uploadinglandmarksActionUnitsMapping(const char* landmarksAUJson);
};
#endif