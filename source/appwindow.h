#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <memory>
#include <vector>

#include "collisiongen.h"
#include "rendermesh.h"
#include "windowbase.h"
#include "viewportwidget.h"
#include "logwidget.h"


class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);

protected:
    void onViewportReady();
    void onImportModelClick();
    void onExportCollisionClick();

    void generateSimpleCollision();
    void generateComplexCollision();
    std::unique_ptr<CollisionGen> collision_gen;

private:
    Ui_MainWindow ui;
    ViewportWidget *viewport_widget;
    LogWidget *log_widget;

    std::vector<std::unique_ptr<Mesh>> model_meshes;
    std::vector<std::unique_ptr<Mesh>> collision_meshes;
    std::vector<std::unique_ptr<RenderMesh>> model_rendermeshes;
    std::vector<std::unique_ptr<RenderMesh>> collision_rendermeshes;
};
#endif
