#include "appwindow.h"
#include "logging.h"
#include "logwidget.h"
#include "modelloader.h"
#include "rendermesh.h"
#include "viewportwidget.h"

#include <memory>
#include <qboxlayout.h>
#include <stdexcept>
#include <vector>

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    this->resize(512, 512);

    Logger::active()->debug("Initialising viewport widget");
	if (!Logger::active())
	{
		throw std::runtime_error("Cannot initialise log widget with no active logger!");
	}

	this->log_widget = new LogWidget(*Logger::active(), this);
	this->ui.ConsoleFrame->setLayout(new QVBoxLayout());
	this->ui.ConsoleFrame->layout()->addWidget(this->log_widget);
	this->ui.ConsoleFrame->layout()->setContentsMargins(0, 0, 0, 0);
	this->ui.ConsoleFrame->layout()->setSpacing(0);
    
	Logger::active()->debug("Initialising viewport widget");
    this->viewport_widget = new ViewportWidget(this);
    this->ui.ViewportFrame->setLayout(new QVBoxLayout());
	this->ui.ViewportFrame->layout()->addWidget(this->viewport_widget);
	this->ui.ViewportFrame->layout()->setContentsMargins(0, 0, 0, 0);
	this->ui.ViewportFrame->layout()->setSpacing(0);
	this->viewport_widget->makeCurrent();
	this->viewport_widget->update();
	
	connect(
		this->viewport_widget,
		&ViewportWidget::graphicsReady,
		this,
		&AppWindow::onViewportReady
	);
}

void AppWindow::onViewportReady()
{

	/// Load default model into the viewport for the time being to better test
	/// viewport logic and onging changes.
	Logger::active()->debug("Loading default viewport model");
	const std::string model_res = ":models/suzanne.usdc";

	std::vector<Mesh> meshes;
	ModelLoader::LoadResourceUSD(model_res, meshes);
	for (const Mesh &mesh : meshes)
	{
		this->model_meshes.push_back(std::make_unique<RenderMesh>(mesh));
		this->viewport_widget->addRenderMesh(this->model_meshes.back().get());
	}
}
