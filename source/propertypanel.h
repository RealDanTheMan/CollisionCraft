#ifndef PROPERTY_PANEL_H
#define PROPERTY_PANEL_H

#include "collisiongen.h"
#include <QWidget>
#include <QPushButton>
#include <QComboBox>

class PropertyPanelWidget : public QWidget
{
    Q_OBJECT

public:
    PropertyPanelWidget(QWidget *parent = nullptr);
    CollisionTechnique getSelectedTechnique() const;

    Q_SIGNAL
    void collisionGenerationRequested();

protected:
    void onTechniqueSelectionChanged();
    CollisionTechnique selected_technique;

private:
    QPushButton *generate_button;
    QComboBox *technique_menu;
};

#endif
