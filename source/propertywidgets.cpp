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
) : QWidget(parent)
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
) : QWidget(parent)
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



/// Dropdown menu property widiget default constructor.
DropdownPropertyWidget::DropdownPropertyWidget(
    const QString &text,
    QWidget *parent
) : QWidget(parent)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);

    this->frame = new QFrame(this);
    this->frame->setLayout(new QHBoxLayout());
    this->frame->layout()->setContentsMargins(0, 0, 0, 0);
    this->frame->layout()->setSpacing(0);

    this->label = new QLabel(text, this->frame);
    this->menu = new QComboBox(this->frame);

    connect(
        this->menu,
        &QComboBox::currentIndexChanged,
        this,
        &DropdownPropertyWidget::onMenuSelectionChanged
    );

    this->layout()->addWidget(this->frame);
    this->frame->layout()->addWidget(this->label);
    this->frame->layout()->addWidget(this->menu);
}

/// Add dropdown menu entry
void DropdownPropertyWidget::addItem(const QString &text, int value)
{
    this->menu->addItem(text, QVariant::fromValue(value));
}

/// Disable or enable this property widget.
void DropdownPropertyWidget::setEnabled(bool enabled)
{
    this->menu->setEnabled(!enabled);
}

/// Set active menu item using item index.
void DropdownPropertyWidget::setSelectedIndex(int index)
{
    this->menu->setCurrentIndex(index);
}

/// Set active menu item using item value.
void DropdownPropertyWidget::setSelected(int value)
{
    int idx = -1;
    for (int i=0; i<this->menu->count(); i++)
    {
        if (this->menu->itemData(i) == value)
        {
            this->setSelectedIndex(i);
            return;
        }
    }
}

/// Get value of currently active menu item.
int DropdownPropertyWidget::getSelected() const
{
    return this->menu->itemData(this->getSelectedIndex()).toInt();
}

/// Get index of currently active menu item.
int DropdownPropertyWidget::getSelectedIndex() const
{
    return this->menu->currentIndex();
}

/// Event handler invoked when the menu changes selection.
void DropdownPropertyWidget::onMenuSelectionChanged(int index)
{
    Q_EMIT this->selectedIndexChanged(index);
    Q_EMIT this->selectedValueChanged(this->menu->itemData(index).toInt());
}
