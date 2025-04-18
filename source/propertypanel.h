#ifndef PROPERTY_PANEL_H
#define PROPERTY_PANEL_H

#include <QWidget>
#include <QPushButton>

class PropertyPanelWidget : public QWidget
{
    Q_OBJECT

public:
    PropertyPanelWidget(QWidget *parent = nullptr);

    Q_SIGNAL
    void collisionGenerationRequested();

protected:
    QPushButton *generate_button;
};

#endif
