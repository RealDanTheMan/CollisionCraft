#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <memory>
#include <vector>

#include "collisiongen.h"
#include "scenemodel.h"
#include "windowbase.h"
#include "viewportwidget.h"
#include "viewportsettingswidget.h"
#include "logwidget.h"
#include "propertypanel.h"


class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);
    void loadModel(const std::string &filepath, bool clear_scene=false);
    void addCollisionModel(const Mesh &collision_mesh);
    void clearAllModels();
    void clearAllCollisionModels();
    void clearScene();

protected:
    void onViewportReady();
    void onImportModelClick();
    void onExportModelsClick();
    void onExportCollisionClick();
    void onFrameAllClick();
    void onCollisionGenerationRequested();
    void onPropertyPanelViewportSettingsChanged(ViewportSettings settings);
    void onViewportSettingsClick();
    void onViewportSettingsApplyClick();
    void onViewportSettingsCancelClick();

    void generateSimpleCollision();
    void generateComplexCollision();
    void generateApproximateCollision();
    void updateViewportSettings(const ViewportSettings &settings);

protected:
    std::unique_ptr<CollisionGen> collision_gen;

private:
    void initWidgets();
    void registerEvents();

    Ui_MainWindow ui;
    ViewportWidget *viewport_widget;
    LogWidget *log_widget;
    PropertyPanelWidget *property_panel;
    QPushButton *viewport_settings_button;
    ViewportSettingsWidget *viewport_settings_widget;

    std::vector<std::unique_ptr<SceneModel>> models;
    std::vector<std::unique_ptr<SceneModel>> collision_models;
};
#endif
