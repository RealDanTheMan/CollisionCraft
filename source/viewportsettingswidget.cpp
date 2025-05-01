#include "viewportsettingswidget.h"
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
        &ViewportSettingsWidget::applyButtonClicked
    );

    connect(
        this->ui.cancel_button,
        &QPushButton::pressed,
        this,
        &ViewportSettingsWidget::cancelButtonClicked
    );
}
