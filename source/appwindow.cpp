#include "appwindow.h"
#include "logging.h"
#include "logwidget.h"

#include <memory>
#include <qboxlayout.h>
#include <stdexcept>

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    this->resize(512, 512);

    Logger::active()->debug("Initialising viewport widget");
	if (!Logger::active())
	{
		throw std::runtime_error("Cannot initialise log widget with no active logger!");
	}

	this->log_widget = std::make_unique<LogWidget>(*Logger::active(), this);
	this->ui.ConsoleFrame->setLayout(new QVBoxLayout());
	this->ui.ConsoleFrame->layout()->addWidget(this->log_widget.get());
	this->ui.ConsoleFrame->layout()->setContentsMargins(0, 0, 0, 0);
	this->ui.ConsoleFrame->layout()->setSpacing(0);
    
	Logger::active()->debug("Initialising viewport widget");
    this->viewport_widget = std::make_unique<ViewportWidget>(this);
    this->ui.ViewportFrame->setLayout(new QVBoxLayout());
	this->ui.ViewportFrame->layout()->addWidget(this->viewport_widget.get());
	this->ui.ViewportFrame->layout()->setContentsMargins(0, 0, 0, 0);
	this->ui.ViewportFrame->layout()->setSpacing(0);

}
