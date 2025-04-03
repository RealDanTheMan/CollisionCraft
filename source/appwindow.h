#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <memory>
#include <vector>

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

private:
    Ui_MainWindow ui;
    ViewportWidget *viewport_widget;
	LogWidget *log_widget;
	std::vector<std::unique_ptr<RenderMesh>> model_meshes;
};
#endif
