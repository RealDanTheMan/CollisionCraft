#include "propertypanel.h"
#include "logging.h"

#include <QVBoxLayout>

PropertyPanelWidget::PropertyPanelWidget(QWidget *parent) : QWidget(parent)
{
    this->generate_button = new QPushButton("Generate Collision");
    this->generate_button->setMinimumHeight(32);

    connect(
        this->generate_button,
        &QPushButton::clicked,
        this,
        &PropertyPanelWidget::collisionGenerationRequested
    );

    QVBoxLayout *panel_layout = new QVBoxLayout();
    panel_layout->addStretch();
    panel_layout->setContentsMargins(QMargins(0.0, 0.0, 0.0, 0.0));
    panel_layout->setSpacing(0);
    panel_layout->addWidget(this->generate_button);

    this->setLayout(panel_layout);
}
