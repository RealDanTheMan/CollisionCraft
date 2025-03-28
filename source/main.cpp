#include <QApplication>
#include <QMainWindow>

#include "appwindow.h"
#include "logging.h"

int main(int argc, char *argv[])
{
    std::string log_file = "/users/mrdan/local/dev/apps/collisioncraft/collisioncraft/app.log";
    std::shared_ptr<Logger> log = std::make_shared<Logger>(log_file);
    log->initLogFile();
    Logger::setActive(log);

    Logger::active()->debug("Starting application");
    QApplication app(argc, argv);

    Logger::active()->debug("Initialising application main window");
    AppWindow win;
    win.setWindowTitle("CollisionCraft");
    win.resize(1000, 720);
    win.show();

    return app.exec();
}
