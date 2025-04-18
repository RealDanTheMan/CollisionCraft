#include "appwindow.h"
#include "logging.h"
#include "logwidget.h"
#include "modelloader.h"
#include "propertypanel.h"
#include "rendermesh.h"
#include "viewportwidget.h"
#include "windowbase.h"

#include <memory>
#include <stdexcept>
#include <vector>

#include <QBoxLayout>
#include <QFileDialog>

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
    this->collision_gen = std::make_unique<CollisionGen>();

    this->ui.setupUi(this);
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

    logDebug("Initialising property panel");
    this->property_panel = new PropertyPanelWidget(this);
    this->ui.PropertyPanelFrame->setLayout(new QVBoxLayout());
    this->ui.PropertyPanelFrame->layout()->addWidget(this->property_panel);
    
    connect(
        this->viewport_widget,
        &ViewportWidget::graphicsReady,
        this,
        &AppWindow::onViewportReady
    );

    connect(
        this->ui.actionImportModel,
        &QAction::triggered,
        this,
        &AppWindow::onImportModelClick
    );

    connect(
        this->ui.actionExportCollision,
        &QAction::triggered,
        this,
        &AppWindow::onExportCollisionClick
    );

    connect(
        this->ui.actionFrameAll,
        &QAction::triggered,
        this,
        &AppWindow::onFrameAllClick
    );
}

/// Event handler invoked when viewport graphics initialisation is completed.
/// Currently loads default test assets & generates test collision.
void AppWindow::onViewportReady()
{

    /// Load default model into the viewport for the time being to better test
    /// viewport logic and onging changes.
    Logger::active()->debug("Loading default viewport model");
    const std::string model_res0 = ":models/teapot.usdc";
    const std::string model_res1 = ":models/arch.usdc";

    std::vector<Mesh> meshes;
    ModelLoader::LoadResourceUSD(model_res0, meshes);
    ModelLoader::LoadResourceUSD(model_res1, meshes);

    for (const Mesh &mesh : meshes)
    {
        this->model_meshes.push_back(std::make_unique<Mesh>(mesh));
        this->model_rendermeshes.push_back(std::make_unique<RenderMesh>(mesh));
        this->viewport_widget->addRenderMesh(this->model_rendermeshes.back().get());
    }

    this->viewport_widget->autoFrameCamera();
    this->generateSimpleCollision();
}

/// Generates simple collision hull for all loaded meshes and adds it to the viewport.
void AppWindow::generateSimpleCollision()
{
    // Clear existing collision.
    for (const std::unique_ptr<RenderMesh>& mesh : this->collision_rendermeshes)
    {
        this->viewport_widget->removeRenderMesh(mesh.get());
    }
    this->collision_rendermeshes.clear();
    this->collision_meshes.clear();

    // Reset collision generator.
    this->collision_gen->clearInputMeshes();
    for (const auto &mesh : this->model_meshes)
    {
        this->collision_gen->addInputMesh(mesh.get());
    }

    /// TODO: Reduce mesh copy operations.
    std::unique_ptr<Mesh> collision;
    this->collision_gen->generateCollisionHull(collision);
    this->collision_meshes.push_back(std::make_unique<Mesh>(*collision));
    this->collision_rendermeshes.push_back(std::make_unique<RenderMesh>(*collision));
    this->collision_rendermeshes.back()->setMaterial(RenderMeshMaterial::Collision);
    this->collision_rendermeshes.back()->setStyle(RenderMeshStyle::ShadedWireframe);
    this->viewport_widget->addRenderMesh(this->collision_rendermeshes.back().get());
}

/// Event handler invoked when user clicks on 'File -> Import Model' menu item.
void AppWindow::onImportModelClick()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Import USD Model", "*.usd*");
    if (!filepath.isEmpty())
    {
        this->model_meshes.clear();
        this->model_rendermeshes.clear();
        this->collision_meshes.clear();
        this->collision_rendermeshes.clear();
        this->viewport_widget->clearRenderMeshes();

        std::vector<Mesh> meshes;
        ModelLoader::LoadResourceUSD(filepath.toStdString(), meshes);

        if (meshes.size() == 0)
        {
            logError("Failed to load usd model -> {}", filepath.toStdString());
            return;
        }

        for (const Mesh &mesh : meshes)
        {
            this->model_meshes.push_back(std::make_unique<Mesh>(mesh));
            this->model_rendermeshes.push_back(std::make_unique<RenderMesh>(mesh));
            this->viewport_widget->addRenderMesh(this->model_rendermeshes.back().get());
        }

        logInfo("Loaded {} meshes into the scene", this->model_meshes.size());
        this->viewport_widget->autoFrameCamera();
    }
}

/// Event handler invoked when user clicks on 'File -> Export Collision' menu item.
void AppWindow::onExportCollisionClick()
{
    QString filepath = QFileDialog::getSaveFileName(this, "Export Collision USD");
    if (!filepath.isEmpty())
    {
        std::vector<const Mesh*> meshes;
        meshes.reserve(this->collision_meshes.size());
        for (const std::unique_ptr<Mesh>& mesh : this->collision_meshes)
        {
            meshes.push_back(mesh.get());
        }

        logInfo("Writing {} meshes to USD file -> {}", meshes.size(), filepath.toStdString());
        ModelLoader::SaveUSD(filepath.toStdString(), meshes);
    }
}

/// Event handler invoked when user clicks on 'View -> Frame All' menu item.
void AppWindow::onFrameAllClick()
{
    this->viewport_widget->autoFrameCamera();
    this->viewport_widget->update();
}
