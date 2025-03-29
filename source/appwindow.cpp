#include "appwindow.h"
#include "logging.h"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    this->resize(512, 512);

    Logger::active()->debug("Initialising viewport widget");
    this->viewport = std::make_unique<ViewportWidget>(this);
    this->viewport->setBackgroundColor(0.1f, 0.1f, 0.4f);
    this->viewport_layout = std::make_unique<QVBoxLayout>(this->ui.ViewportFrame);
    this->viewport_layout->addWidget(this->viewport.get());
    this->ui.ViewportFrame->setLayout(this->viewport_layout.get());
}
