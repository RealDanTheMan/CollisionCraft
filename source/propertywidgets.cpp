#include "propertywidgets.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

/// Decimal property widget default constructor.
DecimalPropertyWidget::DecimalPropertyWidget(
    const QString &text,
    double value,
    double min,
    double max,
    QWidget *parent
)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);

    this->frame = new QFrame(this);
    this->frame->setLayout(new QHBoxLayout());
    this->frame->layout()->setContentsMargins(0, 0, 0, 0);
    this->frame->layout()->setSpacing(0);

    this->label = new QLabel(text, this->frame);
    this->spinner = new QDoubleSpinBox(this->frame);
    this->spinner->setRange(min, max);
    this->spinner->setValue(value);

    this->layout()->addWidget(this->frame);
    this->frame->layout()->addWidget(this->label);
    this->frame->layout()->addWidget(this->spinner);
}

/// Disable or enable this property widget.
void DecimalPropertyWidget::setEnabled(bool enabled)
{
    this->spinner->setReadOnly(!enabled);
}

/// Set value of this property widget spinner element.
void DecimalPropertyWidget::setValue(double value)
{
    this->spinner->setValue(value);
}

/// Get value of this widget spinner element.
double DecimalPropertyWidget::getValue() const
{
    return this->spinner->value();
}


/// Decimal property widget default constructor.
IntegerPropertyWidget::IntegerPropertyWidget(
    const QString &text,
    int value,
    int min,
    int max,
    QWidget *parent
)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);

    this->frame = new QFrame(this);
    this->frame->setLayout(new QHBoxLayout());
    this->frame->layout()->setContentsMargins(0, 0, 0, 0);
    this->frame->layout()->setSpacing(0);

    this->label = new QLabel(text, this->frame);
    this->spinner = new QSpinBox(this->frame);
    this->spinner->setRange(min, max);
    this->spinner->setValue(value);

    this->layout()->addWidget(this->frame);
    this->frame->layout()->addWidget(this->label);
    this->frame->layout()->addWidget(this->spinner);
}

/// Disable or enable this property widget.
void IntegerPropertyWidget::setEnabled(bool enabled)
{
    this->spinner->setReadOnly(!enabled);
}

/// Set value of this property widget spinner element.
void IntegerPropertyWidget::setValue(double value)
{
    this->spinner->setValue(value);
}

/// Get value of this widget spinner element.
double IntegerPropertyWidget::getValue() const
{
    return this->spinner->value();
}
