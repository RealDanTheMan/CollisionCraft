#include "expanderwidget.h"

ExpanderWidget::ExpanderWidget(const QString &title, QWidget *parent)
    :   QWidget(parent),
        toggle_button(new QToolButton),
        content_widget(new QWidget)
{
    this->toggle_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->toggle_button->setArrowType(Qt::UpArrow);
    this->toggle_button->setText(title);
    this->toggle_button->setCheckable(true);
    this->toggle_button->setChecked(true);
    this->toggle_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    connect(
        this->toggle_button,
        &QToolButton::toggled,
        this,
        &ExpanderWidget::onToggleButtonClick
    );

    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);
    this->layout()->addWidget(this->toggle_button);

    this->content_widget->setLayout(new QVBoxLayout());
    this->content_widget->layout()->setContentsMargins(0, 0, 0, 0);
    this->content_widget->layout()->setSpacing(0);
    this->layout()->addWidget(this->content_widget);
}

/// Add widget to this expander contents.
void ExpanderWidget::addWidget(QWidget *widget)
{
    this->content_widget->layout()->addWidget(widget);
    this->content_widget->adjustSize();
}

/// Event handler ivoked when the user clicks on expander toggle button.
void ExpanderWidget::onToggleButtonClick(bool toggle_value)
{
    this->content_widget->adjustSize();
    if (toggle_value)
    {
        this->toggle_button->setArrowType(Qt::UpArrow);
        this->content_widget->setMaximumHeight(this->content_widget->sizeHint().height());
    }
    else
    {
        this->toggle_button->setArrowType(Qt::DownArrow);
        this->content_widget->setMaximumHeight(0);
    }

    this->updateGeometry();
}
