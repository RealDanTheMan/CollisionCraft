#include "propertypanel.h"
#include "collisiongen.h"
#include "logging.h"
#include "expanderwidget.h"

#include <QVBoxLayout>
#include <QLabel>


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

    this->scale_property = new DecimalPropertyWidget(
        "Scale",
        1.0,
        0.01,
        2.0,
        this
    );

    this->resolution_property = new DecimalPropertyWidget(
        "Resolution",
        100000.0,
        1000.0,
        1000000.0,
        this
    );

    this->hull_count_property = new IntegerPropertyWidget(
        "Max Hull Count",
        16,
        1,
        255,
        this
    );

    this->downsampling_property = new IntegerPropertyWidget(
        "Downsampling",
        1,
        1,
        32,
        this
    );

    ExpanderWidget *collision_expander = new ExpanderWidget("Collision Generation", this);
    collision_expander->addWidget(technique_label);
    collision_expander->addWidget(this->technique_menu);
    collision_expander->addWidget(this->scale_property);
    collision_expander->addWidget(this->resolution_property);
    collision_expander->addWidget(this->hull_count_property);
    collision_expander->addWidget(this->downsampling_property);

    QVBoxLayout *panel_layout = new QVBoxLayout();
    panel_layout->setContentsMargins(QMargins(0.0, 0.0, 0.0, 0.0));
    panel_layout->setSpacing(4);
    panel_layout->addWidget(collision_expander);
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

/// Get collision generation settings from property values.
CollisionGenSettings PropertyPanelWidget::getSettings() const
{
    CollisionGenSettings settings;
    settings.scale = this->scale_property->getValue();
    settings.resolution = this->resolution_property->getValue();
    settings.max_hulls = this->hull_count_property->getValue();
    settings.downsample = this->downsampling_property->getValue();

    return settings;
}
