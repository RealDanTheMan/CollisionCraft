#include <filesystem>
#include <QApplication>
#include <QMainWindow>
#include <string>

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
#endif

	return location;
}

int main(int argc, char *argv[])
{
	/// Initialise local app data storage.
    std::string local_app_data = getLocalAppDataLocation();
	std::filesystem::create_directories(local_app_data);

	/// Initialise main logger for this app session.
	std::string log_file = local_app_data + "app.log";
    std::shared_ptr<Logger> log = std::make_shared<Logger>(log_file);
    log->initLogFile();
    Logger::setActive(log);

	/// Create main window and start application loop.
    Logger::active()->debug("Starting application");
    QApplication app(argc, argv);

    Logger::active()->debug("Initialising application main window");
    AppWindow win;
    win.setWindowTitle("CollisionCraft");
    win.resize(1000, 720);
    win.show();

    return app.exec();
}
