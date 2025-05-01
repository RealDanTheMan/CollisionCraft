#ifndef VIEWPORT_SETTINGS_WIDGET_H
#define VIEWPORT_SETTINGS_WIDGET_H

#include "viewportsettingsbase.h"
#include "viewportwidget.h"
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

    void setSettings(const ViewportSettings &settings);
    const ViewportSettings& getSettings() const;

private:
    Ui_ViewportSettingsBase ui;
    ViewportSettings settings;
};

#endif
