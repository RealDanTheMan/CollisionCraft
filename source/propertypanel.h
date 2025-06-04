#ifndef PROPERTY_PANEL_H
#define PROPERTY_PANEL_H

#include "collisiongen.h"
#include "propertywidgets.h"
#include "viewportwidget.h"

#include <QWidget>
#include <QPushButton>
#include <ratio>


class PropertyPanelWidget : public QWidget
{
    Q_OBJECT

public:
    PropertyPanelWidget(QWidget *parent = nullptr);
    CollisionTechnique getSelectedTechnique() const;
    CollisionGenSettings getSettings() const;
    ViewportSettings getViewportSettings() const;

    Q_SIGNAL
    void collisionGenerationRequested();

    Q_SIGNAL
    void viewportSettingsChanged(ViewportSettings settings);

protected:
    void onTechniqueSelectionChanged(int technique);
    void onViewportSettingsPropertyChanged();
    CollisionTechnique selected_technique;

private:
    void initGenerationProperties(QLayout *parent_layout);
    void initCollisionProperties(QLayout *parent_layout);
    void initModelProperties(QLayout *parent_layout);

private:
    QPushButton             *generate_button;
    DropdownPropertyWidget  *technique_menu;

    DecimalPropertyWidget   *scale_property;
    DecimalPropertyWidget   *resolution_property;
    DecimalPropertyWidget   *hull_min_volume_property;
    DecimalPropertyWidget   *concavity_property;
    
    IntegerPropertyWidget   *downsampling_property;
    IntegerPropertyWidget   *hull_count_property;
    IntegerPropertyWidget   *hull_vertex_count_property;
    IntegerPropertyWidget   *depth_property;
    IntegerPropertyWidget   *mode_property;

    TogglePropertyWidget    *collision_hidden_property;
    TogglePropertyWidget    *collision_fill_property;
    TogglePropertyWidget    *collision_wire_property;

    TogglePropertyWidget    *model_hidden_property;
    TogglePropertyWidget    *model_fill_property;
    TogglePropertyWidget    *model_wire_property;
    TogglePropertyWidget    *model_light_property;
};


#endif
