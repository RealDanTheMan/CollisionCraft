#include "propertypanel.h"
#include "collisiongen.h"
#include "logging.h"
#include "expanderwidget.h"
#include "propertywidgets.h"
#include "viewportwidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <cmath>


PropertyPanelWidget::PropertyPanelWidget(QWidget *parent) : 
    QWidget(parent),
    selected_technique(CollisionTechnique::SimpleHull)
{
    QVBoxLayout *panel_layout = new QVBoxLayout();
    panel_layout->setContentsMargins(QMargins(0.0, 0.0, 0.0, 0.0));
    panel_layout->setSpacing(4);

    this->initModelProperties(panel_layout);
    this->initCollisionProperties(panel_layout);
    this->initGenerationProperties(panel_layout);

    panel_layout->addStretch();
    this->setLayout(panel_layout);
}

/// Initial setup of all properties supporting collision generation.
/// Should be called onlly once in the constructor.
void PropertyPanelWidget::initGenerationProperties(QLayout *parent_layout)
{
    this->technique_menu = new DropdownPropertyWidget("Technique", this);
    this->technique_menu->addItem(
        "Simple Hull",
        static_cast<int>(CollisionTechnique::SimpleHull)
    );

    this->technique_menu->addItem(
        "Exact Decomposition",
        static_cast<int>(CollisionTechnique::ExactDecomposition)
    );

    this->technique_menu->addItem(
        "Approximate Decomposition",
        static_cast<int>(CollisionTechnique::ApproximateDecomposition)
    );

    this->scale_property = new DecimalPropertyWidget(
        "Scale",
        1.0,
        0.01,
        2.0,
        0.1,
        2,
        this
    );
    
    this->mode_property = new IntegerPropertyWidget(
        "Mode",
        0,
        0,
        1,
        1,
        this
    );

    this->resolution_property = new DecimalPropertyWidget(
        "Resolution",
        100000.0,
        10000.0,
        64000000.0,
        1000.0,
        0,
        this
    );
    
    this->depth_property = new IntegerPropertyWidget(
        "Depth Planes",
        20,
        1,
        32,
        1,
        this
    );
    
    this->concavity_property = new DecimalPropertyWidget(
        "concavity",
        0.0025,
        0.0,
        1.0,
        0.005,
        4,
        this
    );

    this->hull_count_property = new IntegerPropertyWidget(
        "Max Hull Count",
        16,
        1,
        255,
        1,
        this
    );
    
    this->hull_vertex_count_property = new IntegerPropertyWidget(
        "Max Hull Vertex Count",
        64,
        4,
        1024,
        1,
        this
    );
    
    this->hull_min_volume_property = new DecimalPropertyWidget(
        "Min Hull Volume",
        0.0001,
        0.0,
        0.01,
        0.0001,
        4,
        this
    );

    this->downsampling_property = new IntegerPropertyWidget(
        "Downsampling",
        1,
        1,
        16,
        1,
        this
    );
    
    this->generate_button = new QPushButton("Generate Collision", this);
    this->generate_button->setMinimumHeight(32);

    ExpanderWidget *expander = new ExpanderWidget("Collision Generation", this);
    expander->addWidget(this->technique_menu);
    expander->addWidget(this->mode_property);
    expander->addWidget(this->scale_property);
    expander->addWidget(this->resolution_property);
    expander->addWidget(this->concavity_property);
    expander->addWidget(this->depth_property);
    expander->addWidget(this->hull_count_property);
    expander->addWidget(this->hull_vertex_count_property);
    expander->addWidget(this->hull_min_volume_property);
    expander->addWidget(this->downsampling_property);
    expander->addWidget(this->generate_button);

    parent_layout->addWidget(expander);

    connect(
        this->technique_menu,
        &DropdownPropertyWidget::selectedValueChanged,
        this,
        &PropertyPanelWidget::onTechniqueSelectionChanged
    );

    connect(
        this->generate_button,
        &QPushButton::clicked,
        this,
        &PropertyPanelWidget::collisionGenerationRequested
    );

    /// Poke technique menu to trigger a callback which is going enable/disable
    /// other properties in the panel.
    this->technique_menu->setSelectedIndex(2);
    this->technique_menu->setSelectedIndex(0);
}

/// Initial setup of all properties controlling collision in the viewport.
/// Should be called only once in the constructor.
void PropertyPanelWidget::initCollisionProperties(QLayout *parent_layout)
{
    this->collision_hidden_property = new TogglePropertyWidget("Hidden", false, this);
    this->collision_fill_property = new TogglePropertyWidget("Draw Solid", true, this);
    this->collision_wire_property = new TogglePropertyWidget("Draw Wireframe", true, this);

    ExpanderWidget *expander= new ExpanderWidget("Collision Draw Settings", this);
    expander->addWidget(this->collision_hidden_property);
    expander->addWidget(this->collision_fill_property);
    expander->addWidget(this->collision_wire_property);
    parent_layout->addWidget(expander);

    connect(
        this->collision_hidden_property,
        &TogglePropertyWidget::valueChanged,
        this,
        &PropertyPanelWidget::onViewportSettingsPropertyChanged
    );

    connect(
        this->collision_fill_property,
        &TogglePropertyWidget::valueChanged,
        this,
        &PropertyPanelWidget::onViewportSettingsPropertyChanged
    );

    connect(
        this->collision_wire_property,
        &TogglePropertyWidget::valueChanged,
        this,
        &PropertyPanelWidget::onViewportSettingsPropertyChanged
    );
}

/// Initial setup of all properties controlling models in the viewport.
/// Should be called only once in the constructor.
void PropertyPanelWidget::initModelProperties(QLayout *parent_layout)
{
    this->model_hidden_property = new TogglePropertyWidget("Hidden", false, this);
    this->model_fill_property = new TogglePropertyWidget("Draw Solid", true, this);
    this->model_wire_property = new TogglePropertyWidget("Draw Wireframe", false, this);
    this->model_light_property = new TogglePropertyWidget("Lighting", false, this);

    ExpanderWidget *expander = new ExpanderWidget("Model Draw Settings", this);
    expander->addWidget(model_hidden_property);
    expander->addWidget(model_fill_property);
    expander->addWidget(model_wire_property);
    expander->addWidget(model_light_property);
    parent_layout->addWidget(expander);

    connect(
        this->model_hidden_property,
        &TogglePropertyWidget::valueChanged,
        this,
        &PropertyPanelWidget::onViewportSettingsPropertyChanged
    );

    connect(
        this->model_fill_property,
        &TogglePropertyWidget::valueChanged,
        this,
        &PropertyPanelWidget::onViewportSettingsPropertyChanged
    );

    connect(
        this->model_wire_property,
        &TogglePropertyWidget::valueChanged,
        this,
        &PropertyPanelWidget::onViewportSettingsPropertyChanged
    );

    connect(
        this->model_light_property,
        &TogglePropertyWidget::valueChanged,
        this,
        &PropertyPanelWidget::onViewportSettingsPropertyChanged
    );
}

/// Get currently selected value in collision generation drop-down menu.
CollisionTechnique PropertyPanelWidget::getSelectedTechnique() const
{
    return this->selected_technique;
}

/// Event handler invoked when user changes the 'Collision Generation Technique' menu option.
void PropertyPanelWidget::onTechniqueSelectionChanged(int technique)
{
    this->selected_technique = static_cast<CollisionTechnique>(technique);
    switch (this->selected_technique)
    {
        case CollisionTechnique::SimpleHull:
            this->scale_property->setEnabled(true);
            this->resolution_property->setEnabled(false);
            this->hull_count_property->setEnabled(false);
            this->downsampling_property->setEnabled(false);
            this->mode_property->setEnabled(false);
            this->concavity_property->setEnabled(false);
            this->depth_property->setEnabled(false);
            this->hull_vertex_count_property->setEnabled(false);
            this->hull_min_volume_property->setEnabled(false);
            break;
        case CollisionTechnique::ExactDecomposition:
            this->scale_property->setEnabled(true);
            this->resolution_property->setEnabled(false);
            this->hull_count_property->setEnabled(false);
            this->downsampling_property->setEnabled(false);
            this->mode_property->setEnabled(false);
            this->concavity_property->setEnabled(false);
            this->depth_property->setEnabled(false);
            this->hull_vertex_count_property->setEnabled(false);
            this->hull_min_volume_property->setEnabled(false);
            break;
        case CollisionTechnique::ApproximateDecomposition:
            this->scale_property->setEnabled(true);
            this->resolution_property->setEnabled(true);
            this->hull_count_property->setEnabled(true);
            this->downsampling_property->setEnabled(true);
            this->mode_property->setEnabled(true);
            this->concavity_property->setEnabled(true);
            this->depth_property->setEnabled(true);
            this->hull_vertex_count_property->setEnabled(true);
            this->hull_min_volume_property->setEnabled(true);
            break;
        Default:
            break;
    }

    logDebug("Selected collision technique changed -> {}", this->technique_menu->getSelected());
}

/// Get viewport settings currently active in the property panel.
ViewportSettings PropertyPanelWidget::getViewportSettings() const
{
    ViewportSettings settings = ViewportSettings::Default();
    settings.collision_shaded = this->collision_fill_property->getValue();
    settings.collision_wireframe = this->collision_wire_property->getValue();
    settings.collision_hidden = this->collision_hidden_property->getValue();
    settings.model_shaded = this->model_fill_property->getValue();
    settings.model_wireframe = this->model_wire_property->getValue();
    settings.model_lighting = this->model_light_property->getValue();
    settings.model_hidden = this->model_hidden_property->getValue();

    return settings;
}

/// Event handler invoked when the user edits the values of any properties which
/// control the rendering behavior of the contents in the scene.
void PropertyPanelWidget::onViewportSettingsPropertyChanged()
{
    ViewportSettings settings = this->getViewportSettings();
    Q_EMIT this->viewportSettingsChanged(settings);
}

/// Get collision generation settings from property values.
CollisionGenSettings PropertyPanelWidget::getSettings() const
{
    CollisionGenSettings settings;
    settings.scale = this->scale_property->getValue();
    settings.resolution = this->resolution_property->getValue();
    settings.max_hulls = this->hull_count_property->getValue();
    settings.max_hull_vertices = this->hull_vertex_count_property->getValue();
    settings.min_hull_volume = this->hull_min_volume_property->getValue();
    settings.downsample = this->downsampling_property->getValue();
    settings.mode = this->mode_property->getValue();
    settings.concavity = this->concavity_property->getValue();
    settings.depth_planes = this->depth_property->getValue();

    return settings;
}
