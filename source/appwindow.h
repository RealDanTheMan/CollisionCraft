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
#include "logwidget.h"
#include "propertypanel.h"


class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);
    void loadModel(const std::string &filepath, bool clear_scene=false);
    void clearScene();

protected:
    void onViewportReady();
    void onImportModelClick();
    void onExportCollisionClick();
    void onFrameAllClick();
    void onCollisionGenerationRequested();

    void generateSimpleCollision();
    void generateComplexCollision();
    void generateApproximateCollision();

protected:
    std::unique_ptr<CollisionGen> collision_gen;

private:
    Ui_MainWindow ui;
    ViewportWidget *viewport_widget;
    LogWidget *log_widget;
    PropertyPanelWidget *property_panel;

    std::vector<std::unique_ptr<SceneModel>> models;
    std::vector<std::unique_ptr<SceneModel>> collision_models;
};
#endif
