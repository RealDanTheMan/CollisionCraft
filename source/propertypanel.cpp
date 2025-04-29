#include "propertypanel.h"
#include "collisiongen.h"
#include "logging.h"

#include <QVBoxLayout>
#include <QLabel>


/// Decimal property widget default constructor.
DecimalPropertyWidget::DecimalPropertyWidget(const QString &text, double value, QWidget *parent)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);

    this->frame = new QFrame(this);
    this->frame->setLayout(new QVBoxLayout());
    this->frame->layout()->setContentsMargins(0, 0, 0, 0);
    this->frame->layout()->setSpacing(0);

    this->label = new QLabel(text, this->frame);
    this->spinner = new QDoubleSpinBox(this->frame);
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
IntegerPropertyWidget::IntegerPropertyWidget(const QString &text, int value, QWidget *parent)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);

    this->frame = new QFrame(this);
    this->frame->setLayout(new QVBoxLayout());
    this->frame->layout()->setContentsMargins(0, 0, 0, 0);
    this->frame->layout()->setSpacing(0);

    this->label = new QLabel(text, this->frame);
    this->spinner = new QSpinBox(this->frame);
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


PropertyPanelWidget::PropertyPanelWidget(QWidget *parent) : 
    QWidget(parent),
    selected_technique(CollisionTechnique::SimpleHull)
{
    QLabel *technique_label = new QLabel("Collision Generation Technique", this);

    this->technique_menu = new QComboBox(this);
    this->technique_menu->addItem(
        "Volume Hull",
        QVariant::fromValue(static_cast<int>(CollisionTechnique::SimpleHull))
    );
    this->technique_menu->addItem(
        "Exact Decomposition",
        QVariant::fromValue(static_cast<int>(CollisionTechnique::ExactDecomposition))
    );
    this->technique_menu->addItem(
        "Approximate Decomposition",
        QVariant::fromValue(static_cast<int>(CollisionTechnique::ApproximateDecomposition))
    );

    this->generate_button = new QPushButton("Generate Collision", this);
    this->generate_button->setMinimumHeight(32);

    connect(
        this->generate_button,
        &QPushButton::clicked,
        this,
        &PropertyPanelWidget::collisionGenerationRequested
    );

    connect(
        this->technique_menu,
        &QComboBox::currentIndexChanged,
        this,
        &PropertyPanelWidget::onTechniqueSelectionChanged
    );

    this->scale_property = new DecimalPropertyWidget("Collision Scale", 1.0, this);
    this->resolution_property = new DecimalPropertyWidget("Collision Resolution", 100000.0, this);
    this->hull_count_property = new IntegerPropertyWidget("Collision Maximum Hull Count", 16, this);
    this->downsampling_property = new IntegerPropertyWidget("Collision Downsampling", 1, this);

    QVBoxLayout *panel_layout = new QVBoxLayout();
    panel_layout->setContentsMargins(QMargins(0.0, 0.0, 0.0, 0.0));
    panel_layout->setSpacing(4);
    panel_layout->addWidget(technique_label);
    panel_layout->addWidget(this->technique_menu);
    panel_layout->addWidget(this->scale_property);
    panel_layout->addWidget(this->resolution_property);
    panel_layout->addWidget(this->hull_count_property);
    panel_layout->addWidget(this->downsampling_property);
    panel_layout->addStretch();
    panel_layout->addWidget(this->generate_button);

    this->setLayout(panel_layout);
}

/// Get currently selected value in collision generation drop-down menu.
CollisionTechnique PropertyPanelWidget::getSelectedTechnique() const
{
    return this->selected_technique;
}

/// Event handler invoked when user changes the 'Collision Generation Technique' menu option.
void PropertyPanelWidget::onTechniqueSelectionChanged()
{
    const int idx = this->technique_menu->currentIndex();
    const int technique_id = this->technique_menu->itemData(idx).toInt();

    this->selected_technique = static_cast<CollisionTechnique>(technique_id);
    switch (this->selected_technique)
    {
        case CollisionTechnique::SimpleHull:
            this->scale_property->setEnabled(true);
            this->resolution_property->setEnabled(false);
            this->hull_count_property->setEnabled(false);
            this->downsampling_property->setEnabled(false);
            break;
        case CollisionTechnique::ExactDecomposition:
            this->scale_property->setEnabled(true);
            this->resolution_property->setEnabled(false);
            this->hull_count_property->setEnabled(false);
            this->downsampling_property->setEnabled(false);
            break;
        case CollisionTechnique::ApproximateDecomposition:
            this->scale_property->setEnabled(true);
            this->resolution_property->setEnabled(true);
            this->hull_count_property->setEnabled(true);
            this->downsampling_property->setEnabled(true);
            break;
        Default:
            break;
    }
    logDebug("Selected collision technique changed -> {}", technique_id);
}
