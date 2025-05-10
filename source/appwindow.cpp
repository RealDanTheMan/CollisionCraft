#include "appwindow.h"
#include "collisiongen.h"
#include "logging.h"
#include "logwidget.h"
#include "modelloader.h"
#include "propertypanel.h"
#include "rendermesh.h"
#include "scenemodel.h"
#include "viewportwidget.h"
#include "windowbase.h"

#include <memory>
#include <stdexcept>
#include <vector>

#include <QBoxLayout>
#include <QStackedLayout>
#include <QFileDialog>
#include <QPushButton>

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

    this->initWidgets();
    this->registerEvents();
}

/// Create and append all UI widgets to this window.
void AppWindow::initWidgets()
{
    this->log_widget = new LogWidget(*Logger::active(), this);
    this->ui.ConsoleFrame->setLayout(new QVBoxLayout());
    this->ui.ConsoleFrame->layout()->addWidget(this->log_widget);
    this->ui.ConsoleFrame->layout()->setContentsMargins(0, 0, 0, 0);
    this->ui.ConsoleFrame->layout()->setSpacing(0);
    
    Logger::active()->debug("Initialising viewport widget");
    this->viewport_widget = new ViewportWidget(this->ui.ViewportFrame);
    this->ui.ViewportFrame->setLayout(new QStackedLayout());
    this->ui.ViewportFrame->layout()->addWidget(this->viewport_widget);
    this->ui.ViewportFrame->layout()->setContentsMargins(0, 0, 0, 0);
    this->ui.ViewportFrame->layout()->setSpacing(0);
    this->viewport_widget->makeCurrent();
    this->viewport_widget->update();

    logDebug("Initialising property panel");
    this->property_panel = new PropertyPanelWidget(this);
    this->ui.PropertyPanelFrame->setLayout(new QVBoxLayout());
    this->ui.PropertyPanelFrame->layout()->setContentsMargins(0, 0, 0, 0);
    this->ui.PropertyPanelFrame->layout()->setSpacing(0);
    this->ui.PropertyPanelFrame->layout()->addWidget(this->property_panel);
}

/// Enable and connect UI event across all widgets.
void AppWindow::registerEvents()
{
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
        this->ui.actionExportModels,
        &QAction::triggered,
        this,
        &AppWindow::onExportModelsClick
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

    connect(
        this->property_panel,
        &PropertyPanelWidget::viewportSettingsChanged,
        this,
        &AppWindow::onPropertyPanelViewportSettingsChanged
    );
}

/// Unloads and removes any active standard models in the current scene.
void AppWindow::clearAllModels()
{
    logDebug("Clearing all {} standard models in the scene", this->models.size());
    for (const std::unique_ptr<SceneModel> &model : this->models)
    {
        this->viewport_widget->removeRenderMesh(&model->getRenderMesh());
    }

    this->models.clear();
}

/// Unloads and removes any active collision models in the current scene.
void AppWindow::clearAllCollisionModels()
{

    logDebug("Clearing all {} collision models in the scene", this->collision_models.size());
    for (const std::unique_ptr<SceneModel> &model : this->collision_models)
    {
        this->viewport_widget->removeRenderMesh(&model->getRenderMesh());
    }

    this->collision_models.clear();
}

/// Unloads all models and collisions from active scene.
void AppWindow::clearScene()
{
    this->clearAllModels();
    this->clearAllCollisionModels();
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

    this->viewport_widget->makeCurrent();
    for (const Mesh &mesh : meshes)
    {
        this->models.push_back(std::make_unique<SceneModel>(mesh));
        this->viewport_widget->addRenderMesh(&this->models.back()->getRenderMesh());
    }
}

/// Create collision model from given mesh and add it to current scene.
void AppWindow::addCollisionModel(const Mesh &collision_mesh)
{
    this->viewport_widget->makeCurrent();
    this->collision_models.push_back(std::make_unique<SceneModel>(collision_mesh));
    this->collision_models.back()->getRenderMesh().setMaterial(RenderMeshMaterial::Collision);
    this->collision_models.back()->getRenderMesh().setStyle(RenderMeshStyle::ShadedWireframe);
    this->viewport_widget->addRenderMesh(&this->collision_models.back()->getRenderMesh());
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

    CollisionGenSettings settings = this->property_panel->getSettings();
    std::unique_ptr<Mesh> collision;
    this->collision_gen->generateCollisionHull(settings, collision);
    if (collision)
    {
        this->addCollisionModel(*collision);
    }
}

/// Generate complex collision hulls using exact convex decomposition technique.
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

    CollisionGenSettings settings = this->property_panel->getSettings();
    std::vector<std::unique_ptr<Mesh>> collisions;
    this->collision_gen->generateCollisionHulls(settings, collisions);

    logInfo("Generated {} complex collision meshes", collisions.size());
    this->viewport_widget->makeCurrent();
    for (const auto &collision : collisions)
    {
        this->addCollisionModel(*collision);
    }
}

/// Generate approximate collision using approximate convex decomposition technique.
void AppWindow::generateApproximateCollision()
{
    logDebug("Generating scene approximate collision");

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

    CollisionGenSettings settings = this->property_panel->getSettings();
    std::vector<std::unique_ptr<Mesh>> collisions;
    this->collision_gen->generateVHACD(settings, collisions);

    logInfo("Generated {} approximate collision meshes", collisions.size());
    this->viewport_widget->makeCurrent();
    for (const auto &collision : collisions)
    {
        this->addCollisionModel(*collision);
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

/// Event handler invoked when user click on 'File -> Export Models' menu item.
void AppWindow::onExportModelsClick()
{
    QString filepath = QFileDialog::getSaveFileName(this, "Export Models To USD");
    if (!filepath.isEmpty())
    {
        std::vector<const Mesh*> meshes;
        meshes.reserve(this->models.size());
        for (const auto &model : this->models)
        {
            meshes.push_back(&model->getMesh());
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
    CollisionGenSettings settings = this->property_panel->getSettings();
    switch (technique)
    {
        case CollisionTechnique::SimpleHull:
            this->generateSimpleCollision();
            break;
        case CollisionTechnique::ExactDecomposition:
            this->generateComplexCollision();
            break;
        case CollisionTechnique::ApproximateDecomposition:
            this->generateApproximateCollision();
            break;
        default:
            logError("Invalid collision generation technique -> {}", static_cast<int>(technique));
            return;
    }

    this->viewport_widget->update();
}

/// Event handler invoked when the user updates any of the viewport settings properties on
/// the property panel.
void AppWindow::onPropertyPanelViewportSettingsChanged(ViewportSettings settings)
{
    this->updateViewportSettings(settings);
}

/// Update the active viewport settings.
/// Viewport settings control the rendering behavior of models and collision in the scene.
void AppWindow::updateViewportSettings(const ViewportSettings &settings)
{
    logDebug("Updating viewport settings");
    RenderMeshStyle collision_style;
    RenderMeshStyle model_style;

    /// Update collision object rendering style.
    if (settings.collision_shaded && settings.collision_wireframe)
    {
        collision_style = RenderMeshStyle::ShadedWireframe;
    }
    else if (!settings.collision_shaded && settings.collision_wireframe)
    {
        collision_style = RenderMeshStyle::WireframeOnly;
    }
    else
    {
        collision_style = RenderMeshStyle::Shaded;
    }

    for (const std::unique_ptr<SceneModel> &collision : this->collision_models)
    {
        collision->getRenderMesh().setStyle(collision_style);
        collision->getRenderMesh().setVisibility(!settings.collision_hidden);
    }

    /// Update standard models rendering style.
    if (settings.model_shaded && settings.model_wireframe)
    {
        model_style = RenderMeshStyle::ShadedWireframe;
    }
    else if (!settings.model_shaded && settings.model_wireframe)
    {
        model_style = RenderMeshStyle::WireframeOnly;
    }
    else
    {
        model_style = RenderMeshStyle::Shaded;
    }

    for (const std::unique_ptr<SceneModel> &model : this->models)
    {   
        model->getRenderMesh().setStyle(model_style);
        model->getRenderMesh().setVisibility(!settings.model_hidden);
    }

    this->viewport_widget->update();
}
