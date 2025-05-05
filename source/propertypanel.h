#ifndef PROPERTY_PANEL_H
#define PROPERTY_PANEL_H

#include "collisiongen.h"
#include "propertywidgets.h"

#include <QWidget>
#include <QPushButton>


class PropertyPanelWidget : public QWidget
{
    Q_OBJECT

public:
    PropertyPanelWidget(QWidget *parent = nullptr);
    CollisionTechnique getSelectedTechnique() const;
    CollisionGenSettings getSettings() const;

    Q_SIGNAL
    void collisionGenerationRequested();

protected:
    void onTechniqueSelectionChanged();
    CollisionTechnique selected_technique;

private:
    QPushButton *generate_button;
    QComboBox *technique_menu;
    DecimalPropertyWidget *scale_property;
    DecimalPropertyWidget *resolution_property;
    IntegerPropertyWidget *hull_count_property;
    IntegerPropertyWidget *downsampling_property;
};


#endif
