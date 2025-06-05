#include "propertypanel.h"
#include "collisiongen.h"
#include "expanderwidget.h"
#include "propertywidgets.h"
#include "viewportwidget.h"

#include <QVBoxLayout>
#include <QLabel>


PropertyPanelWidget::PropertyPanelWidget(QWidget *parent) : 
    QWidget(parent)
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
    this->scale_property = new DecimalPropertyWidget(
        "Scale",
        1.0,
        0.01,
        2.0,
        0.1,
        2,
        "Final scale of each convex hull",
        this
    );
    
    this->mode_property = new IntegerPropertyWidget(
        "Mode",
        0,
        0,
        1,
        1,
        "Decomposition Mode - 0=Volume, 1=Tetrahedron",
        this
    );

    this->resolution_property = new DecimalPropertyWidget(
        "Resolution",
        100000.0,
        10000.0,
        64000000.0,
        1000.0,
        0,
        "Max volumes used during decomposition",
        this
    );
    
    this->depth_property = new IntegerPropertyWidget(
        "Depth Planes",
        20,
        1,
        32,
        1,
        "Maximum number of clip planes using during decomposition",
        this
    );
    
    this->concavity_property = new DecimalPropertyWidget(
        "Concavity",
        0.0025,
        0.0,
        1.0,
        0.005,
        4,
        "Maximum concavity factor",
        this
    );

    this->hull_count_property = new IntegerPropertyWidget(
        "Max Hull Count",
        16,
        1,
        255,
        1,
        "Maximum number of generated convex hull elements",
        this
    );
    
    this->hull_vertex_count_property = new IntegerPropertyWidget(
        "Max Hull Vertex Count",
        64,
        4,
        1024,
        1,
        "Maximum number of vertices in each convex hull generated",
        this
    );
    
    this->hull_min_volume_property = new DecimalPropertyWidget(
        "Min Hull Volume",
        0.0001,
        0.0,
        0.01,
        0.0001,
        4,
        "Adaptive sampling of the generated convex hulls",
        this
    );

    this->downsampling_property = new IntegerPropertyWidget(
        "Downsampling",
        1,
        1,
        16,
        1,
        "Precision of convex hull generation",
        this
    );
    
    this->generate_button = new QPushButton("Generate Collision", this);
    this->generate_button->setMinimumHeight(32);

    ExpanderWidget *expander = new ExpanderWidget("Collision Generation", this);
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
        this->generate_button,
        &QPushButton::clicked,
        this,
        &PropertyPanelWidget::collisionGenerationRequested
    );
}

/// Initial setup of all properties controlling collision in the viewport.
/// Should be called only once in the constructor.
void PropertyPanelWidget::initCollisionProperties(QLayout *parent_layout)
{
    this->collision_hidden_property = new TogglePropertyWidget(
        "Hidden",
        false,
        "Hide collision",
        this
    );

    this->collision_fill_property = new TogglePropertyWidget(
        "Draw Solid",
        true,
        "Draw collision triangles",
        this
    );

    this->collision_wire_property = new TogglePropertyWidget(
        "Draw Wireframe",
        true,
        "Draw collision Wireframe",
        this
    );

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
    this->model_hidden_property = new TogglePropertyWidget(
        "Hidden",
        false, 
        "Hide scene models",
        this
    );

    this->model_fill_property = new TogglePropertyWidget(
        "Draw Solid",
        true,
        "Draw model triangles",
        this
    );

    this->model_wire_property = new TogglePropertyWidget(
        "Draw Wireframe",
        false,
        "Draw model Wireframe",
        this
    );

    this->model_light_property = new TogglePropertyWidget(
        "Lighting",
        false,
        "Render model directional Lighting",
        this
    );

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
