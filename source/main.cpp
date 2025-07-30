#include <filesystem>
#include <QApplication>

#include <QMainWindow>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <qapplication.h>
#include <qcontainerfwd.h>
#include <string>

#include <pxr/base/tf/diagnosticMgr.h>

#include "appwindow.h"
#include "logging.h"

/// Get suitable location for local app data storage.
/// For the time being we only support MacOS and Linux.
std::string getLocalAppDataLocation()
{
    std::string location = "";
    const char *home = std::getenv("HOME");

#if defined(__APPLE__)
    if (home)
    {
        location = std::string(home) + "/Library/Application Support/CollisionCraft/";
    }

#elif defined(__linux__)
    const char *xdg_home = std::getenv("XDG_DATA_HOME");
    if (xdg_home)
    {
        location = std::string(xdg_home) + "/CollisionCraft/";
    }
    else if (home)
    {
        location = std::string(home) + "/.local/CollisionCraft/";
    }

#elif defined(_WIN32)
    const char * app_data = std::getenv("LOCALAPPDATA");
    location = std::string(app_data) + "/CollisionCraft/";
#endif

    return location;
}

/// Loads and applies given QSS stylesheet to given QT app.
/// @param: app Application to apply stylesheet to.
/// @param: filepath Filepath to the qss stylesheet file.
bool applyStyleSheet(QApplication& app, const std::string &filepath)
{
    QFile file(QString::fromStdString(filepath));
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QString contents = file.readAll();
        app.setStyleSheet(contents);

        return true;
    }

    return false;
}

int main(int argc, char *argv[])
{
    /// Enable USD debug output.
    //pxr::TfDiagnosticMgr::GetInstance().EnableNotification2();
    //pxr::TfDiagnosticMgr::GetInstance().SetQuiet(false);

    /// Initialise local app data storage.
    std::string local_app_data = getLocalAppDataLocation();
    std::filesystem::create_directories(local_app_data);

    /// Initialise main logger for this app session.
    std::string log_file = local_app_data + "app.log";
    std::shared_ptr<Logger> log = std::make_shared<Logger>(log_file);
    log->initLogFile();
    Logger::setActive(log);

#ifdef DEBUG
    Logger::active()->setDebugEnabled(true);
#endif
    
#if defined(__linux__)
    /// Force X11 session & disable window alpha to avoid issues with window alpha sorting
    /// when dealing with many window composititors available on linux distros.
    qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));
    qputenv("QT_WAYLAND_DISABLE_WINDOW_ALPHA", QByteArray("1"));
#endif
#if defined(_WIN32)
    // Resolve default location for QT plugins shipped with the application.
    QString exe_path = QFileInfo(QString::fromLocal8Bit(argv[0])).absolutePath();
    QString plugin_path = QDir(exe_path + "/qt/plugins").absolutePath();
    QCoreApplication::addLibraryPath(plugin_path);

    qputenv("PXR_PLUGINPATH_NAME", "\\usd");
#endif

    /// Define OpenGL context specification.
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
    format.setVersion(3, 3);
    format.setDepthBufferSize(32);
    format.setAlphaBufferSize(8);
    format.setStencilBufferSize(8);
    format.setSamples(4);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(format);

    /// Create main window and start application loop.
    Logger::active()->debug("Starting application");
    QApplication::setStyle("Fusion");
    QApplication app(argc, argv);

    std::string stylesheet = ":/ui/stylesheet.qss";
    if(!applyStyleSheet(app, stylesheet))
    {
        Logger::active()->warning("Failed to load default application stylesheet");
    }

    logInfo("Theme style loaded -> {}", app.style()->objectName().toStdString());
    Logger::active()->debug("Initialising application main window");
    AppWindow win;
    win.setWindowTitle("Collision Craft");
    win.resize(1000, 720);
    win.show();

    return app.exec();
}
