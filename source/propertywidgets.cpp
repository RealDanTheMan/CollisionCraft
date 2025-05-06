#include "propertywidgets.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

/// Toggle property widget default constructor.
TogglePropertyWidget::TogglePropertyWidget(
    const QString &text,
    bool value,
    QWidget *parent
) : QWidget(parent)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);

    this->frame = new QFrame(this);
    this->layout()->addWidget(this->frame);

    this->label = new QLabel(text, this->frame);
    this->toggle = new QCheckBox(this->frame);
    this->toggle->setCheckable(true);
    this->toggle->setChecked(value);

    connect(
        this->toggle,
        &QCheckBox::checkStateChanged,
        this,
        &TogglePropertyWidget::valueChanged
    );

    QHBoxLayout *frame_layout = new QHBoxLayout();
    frame_layout->setContentsMargins(0, 0, 0, 0);
    frame_layout->setSpacing(0);
    frame_layout->addWidget(this->label);
    frame_layout->addStretch();
    frame_layout->addWidget(this->toggle);
    this->frame->setLayout(frame_layout);
}

/// Disable or enable this property widget.
void TogglePropertyWidget::setEnabled(bool enabled)
{
    this->toggle->setEnabled(enabled);
}

/// Set value of this toggle property.
void TogglePropertyWidget::setValue(bool value)
{
    this->toggle->setChecked(value);
}

/// Get value of this toggle property.
bool TogglePropertyWidget::getValue() const
{
    return this->toggle->isChecked();
}


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
    this->layout()->addWidget(this->frame);

    this->label = new QLabel(text, this->frame);
    this->spinner = new QDoubleSpinBox(this->frame);
    this->spinner->setMinimumWidth(128);
    this->spinner->setRange(min, max);
    this->spinner->setValue(value);

    QHBoxLayout *frame_layout = new QHBoxLayout();
    frame_layout->setContentsMargins(0, 0, 0, 0);
    frame_layout->setSpacing(0);
    frame_layout->addWidget(this->label);
    frame_layout->addStretch();
    frame_layout->addWidget(this->spinner);
    this->frame->setLayout(frame_layout);
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
    this->layout()->addWidget(this->frame);

    this->label = new QLabel(text, this->frame);
    this->spinner = new QSpinBox(this->frame);
    this->spinner->setMinimumWidth(128);
    this->spinner->setRange(min, max);
    this->spinner->setValue(value);

    QHBoxLayout *frame_layout = new QHBoxLayout();
    frame_layout->setContentsMargins(0, 0, 0, 0);
    frame_layout->setSpacing(0);
    frame_layout->addWidget(this->label);
    frame_layout->addStretch();
    frame_layout->addWidget(this->spinner);
    this->frame->setLayout(frame_layout);
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
    this->layout()->addWidget(this->frame);

    this->label = new QLabel(text, this->frame);
    this->menu = new QComboBox(this->frame);
    this->menu->setMinimumWidth(128);

    connect(
        this->menu,
        &QComboBox::currentIndexChanged,
        this,
        &DropdownPropertyWidget::onMenuSelectionChanged
    );

    QHBoxLayout *frame_layout = new QHBoxLayout();
    frame_layout->setContentsMargins(0, 0, 0, 0);
    frame_layout->setSpacing(0);
    frame_layout->addWidget(this->label);
    frame_layout->addStretch();
    frame_layout->addWidget(this->menu);
    this->frame->setLayout(frame_layout);
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
