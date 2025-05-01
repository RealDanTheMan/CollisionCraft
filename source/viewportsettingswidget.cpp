#include "viewportsettingswidget.h"
#include "viewportwidget.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>

ViewportSettingsWidget::ViewportSettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    this->ui.setupUi(this);

    connect(
        this->ui.apply_button,
        &QPushButton::pressed,
        this,
        &ViewportSettingsWidget::onApplyClick
    );

    connect(
        this->ui.cancel_button,
        &QPushButton::pressed,
        this,
        &ViewportSettingsWidget::cancelButtonClicked
    );
}

void ViewportSettingsWidget::setSettings(const ViewportSettings &settings)
{
    this->settings = settings;
    this->ui.collision_shaded_checkbox->setChecked(this->settings.collisionShaded);
    this->ui.collision_wireframe_checkbox->setChecked(this->settings.collisionWireframe);
    this->ui.model_shaded_checkbox->setChecked(this->settings.modelShaded);
    this->ui.model_wireframe_checkbox->setChecked(this->settings.modelWireframe);
    this->ui.model_lighting_checkbox->setChecked(this->settings.modelLighting);
}

const ViewportSettings& ViewportSettingsWidget::getSettings() const
{
    return this->settings;
}

void ViewportSettingsWidget::updateSettings()
{
    this->settings.collisionShaded = this->ui.collision_shaded_checkbox->isChecked();
    this->settings.collisionWireframe = this->ui.collision_wireframe_checkbox->isChecked();
    this->settings.modelShaded = this->ui.model_shaded_checkbox->isChecked();
    this->settings.modelWireframe = this->ui.model_wireframe_checkbox->isChecked();
    this->settings.modelLighting = this->ui.model_lighting_checkbox->isChecked();
}

void ViewportSettingsWidget::onApplyClick()
{
    this->updateSettings();
    Q_EMIT this->applyButtonClicked();
}
