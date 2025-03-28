#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "windowbase.h"
#include "viewportwidget.h"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);

private:
    Ui_MainWindow ui;
    std::unique_ptr<ViewportWidget> viewport;
    std::unique_ptr<QVBoxLayout> viewport_layout;
};
#endif
