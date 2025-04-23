#include "appwindow.h"
#include "collisiongen.h"
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

    connect(
        this->property_panel,
        &PropertyPanelWidget::collisionGenerationRequested,
        this,
        &AppWindow::onCollisionGenerationRequested
    );
}

/// Unloads all models and collisions from active scene.
void AppWindow::clearScene()
{
    logDebug("Clearing active scene content");
    this->viewport_widget->clearRenderMeshes();
    this->viewport_widget->update();
    this->models.clear();
    this->collision_models.clear();
}

/// Loads model from asset file.
/// @param: filepath Location of model file on disk on in app resources.
/// @param: clear_scene Reset active scene before loading.
void AppWindow::loadModel(const std::string &filepath, bool clear_scene)
{
    if (clear_scene)
    {
        this->clearScene();
    }

    logDebug("Loading model file -> {}", filepath);
    std::vector<Mesh> meshes;
    ModelLoader::LoadResourceUSD(filepath, meshes);
    logDebug("Loaded {} meshes from file -> {}", meshes.size(), filepath);

    for (const Mesh &mesh : meshes)
    {
        this->models.push_back(std::make_unique<SceneModel>(mesh));
        this->viewport_widget->addRenderMesh(&this->models.back()->getRenderMesh());
    }
}

/// Event handler invoked when viewport graphics initialisation is completed.
/// Currently loads default test assets & generates test collision.
void AppWindow::onViewportReady()
{

    /// Load default model into the viewport for the time being to better test
    /// viewport logic and onging changes.
    Logger::active()->debug("Loading default viewport model");
    this->loadModel(":models/arch.usdc");

    this->viewport_widget->setBackgroundColor(0.15, 0.15, 0.15);
    this->viewport_widget->autoFrameCamera();
    this->viewport_widget->update();
}


/// Generates simple collision hull for all loaded meshes and adds it to the viewport.
void AppWindow::generateSimpleCollision()
{
    logDebug("Generating scene simple collision");

    // Clear existing collision.
    for (const auto &collision : this->collision_models)
    {
        this->viewport_widget->removeRenderMesh(&collision->getRenderMesh());
    }
    this->collision_models.clear();

    // Reset collision generator.
    this->collision_gen->clearInputMeshes();
    for (const auto &model : this->models)
    {
        this->collision_gen->addInputMesh(&model->getMesh());
    }

    std::unique_ptr<Mesh> collision;
    this->collision_gen->generateCollisionHull(collision);
    this->collision_models.push_back(std::make_unique<SceneModel>(*collision));

    RenderMesh &rmesh = this->collision_models.back()->getRenderMesh();
    rmesh.setMaterial(RenderMeshMaterial::Collision);
    rmesh.setStyle(RenderMeshStyle::ShadedWireframe);
    this->viewport_widget->addRenderMesh(&rmesh);
}

void AppWindow::generateComplexCollision()
{
    logDebug("Generating scene complex collision");

    // Clear existing collision.
    for (const auto &collision : this->collision_models)
    {
        this->viewport_widget->removeRenderMesh(&collision->getRenderMesh());
    }
    this->collision_models.clear();

    // Reset collision generator.
    this->collision_gen->clearInputMeshes();
    for (const auto &model : this->models)
    {
        this->collision_gen->addInputMesh(&model->getMesh());
    }

    std::vector<std::unique_ptr<Mesh>> collisions;
    this->collision_gen->generateCollisionHulls(collisions);

    logInfo("Generated {} complex collision meshes", collisions.size());
    for (const auto &collision : collisions)
    {
        this->collision_models.push_back(std::make_unique<SceneModel>(*collision));

        RenderMesh &rmesh = this->collision_models.back()->getRenderMesh();
        rmesh.setMaterial(RenderMeshMaterial::Collision);
        rmesh.setStyle(RenderMeshStyle::ShadedWireframe);
        this->viewport_widget->addRenderMesh(&rmesh);
    }
}

/// Event handler invoked when user clicks on 'File -> Import Model' menu item.
void AppWindow::onImportModelClick()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Import USD Model", "*.usd*");
    if (!filepath.isEmpty())
    {
        this->loadModel(filepath.toStdString(), true);
        this->viewport_widget->autoFrameCamera();
        this->viewport_widget->update();
    }
}

/// Event handler invoked when user clicks on 'File -> Export Collision' menu item.
void AppWindow::onExportCollisionClick()
{
    QString filepath = QFileDialog::getSaveFileName(this, "Export Collision USD");
    if (!filepath.isEmpty())
    {
        std::vector<const Mesh*> meshes;
        meshes.reserve(this->collision_models.size());
        for (const auto &collision : this->collision_models)
        {
            meshes.push_back(&collision->getMesh());
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

/// Event handler invoked when user requests new collision generation.
void AppWindow::onCollisionGenerationRequested()
{
    CollisionTechnique technique = this->property_panel->getSelectedTechnique();
    switch (technique)
    {
        case CollisionTechnique::SimpleHull:
            this->generateSimpleCollision();
            break;
        case CollisionTechnique::ExactDecomposition:
            this->generateComplexCollision();
            break;
        default:
            logError("Invalid collision generation technique -> {}", static_cast<int>(technique));
            return;
    }

    this->viewport_widget->update();
}
