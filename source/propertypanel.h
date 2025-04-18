#ifndef PROPERTY_PANEL_H
#define PROPERTY_PANEL_H

#include <QWidget>
#include <QPushButton>

class PropertyPanelWidget : public QWidget
{
public:
    PropertyPanelWidget(QWidget *parent = nullptr);

protected:
    QPushButton *generate_button;
};

#endif
