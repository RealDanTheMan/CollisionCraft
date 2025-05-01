#ifndef VIEWPORT_SETTINGS_WIDGET_H
#define VIEWPORT_SETTINGS_WIDGET_H

#include "viewportsettingsbase.h"
#include <QWidget>

class ViewportSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    ViewportSettingsWidget(QWidget *parent = nullptr);

    Q_SIGNAL
    void cancelButtonClicked();

    Q_SIGNAL
    void applyButtonClicked();

private:
    Ui_ViewportSettingsBase ui;
};

#endif
