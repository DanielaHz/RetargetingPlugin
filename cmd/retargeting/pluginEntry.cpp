// Core Maya
#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MSyntax.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>

// Qt Interface
#include <QtCore/QPointer>
#include "interface/widgets/PixelMuxPluginWindow.h"
#include <filesystem>

#define kFlagModel      "-m"
#define kFlagLongModel  "--model"
#define kFlagAudio      "-a"
#define kFlagLongAudio  "--audio"
#define kFlagImage      "-i"
#define kFlagLongImage  "--image"

static QPointer<PixelMuxWindow> plugin_window;  

class buildUIPanel : public MPxCommand
{
public:
    buildUIPanel() {}
    virtual ~buildUIPanel() {}
    static void* creator() { return new buildUIPanel(); }
    MStatus doIt(const MArgList& args) override;
    bool isUndoable() const override { return false; }
};

class PluginContext {

public:
    static void setPluginDir(const std::string& dir){pluginDir = dir;};
    static const std::string& getPluginDir() {return pluginDir;};

private:
    static std::string pluginDir;
};

std::string PluginContext::pluginDir = "";  

MStatus buildUIPanel::doIt(const MArgList&) {
    std::string pluginDir = PluginContext::getPluginDir();
    if (!plugin_window) {
        plugin_window = new PixelMuxWindow(pluginDir);
    }

    plugin_window->show();
    plugin_window->raise(); // overlap the window
    plugin_window->activateWindow();

    return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
    MFnPlugin plugin(obj, "PixelMux Retargeting Plug-in", "1.0", "Any");
    std::string pluginPath =  plugin.loadPath().asChar();
    std::string pluginDir = std::filesystem::path(pluginPath).parent_path().string();

    PluginContext::setPluginDir(pluginDir);

    plugin.registerCommand("buildUIPanel", buildUIPanel::creator);
    return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin plugin(obj);
    plugin.deregisterCommand("buildUIPanel");
    return MS::kSuccess;
}
